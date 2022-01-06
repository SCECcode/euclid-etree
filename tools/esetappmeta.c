/* $Idw$
 *
 * Etree metadata manipulation program.
 * Language: C
 *
 * Copyright (C) 2003		Julio Lopez	All Rights Reserved.
 * See disclaim.txt for disclaimer and copyright information.
 */

/*
 * Options:
 *	etree_file	name of the etree file (required)
 *	metadata_file	specify a file containing the metdata (optional)
 *
 * Notes:
 *   We assume that the metadata file only contains text.  The etree
 *   metadata will be set to the contents from the beginning of the input
 *   metadata file up to the first nil '\0' character or EOF, whichever
 *   comes first.
 *
 * @todo:
 *	- add quiet (-q) flag
 *	- check maximum metadata file size
 */

#include "etree.h"


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
/*#include <sys/limits.h>*/

/*
 * static global variables to store program options
 */
static const char* _opt_program_name	 = NULL;
static const char* _opt_etreefile	 = NULL;
static const char* _opt_metafile	 = NULL;


static void
usage (
	const char* program_name,
	const char* usage_msg,
	int	    exitcode,
	const char* message_format,
	...
	)
{
    va_list ap;

    if (NULL != message_format) {
	if (NULL != program_name) {
	    fputs (program_name, stderr);
	    fputc (':', stderr);
	}

	va_start (ap, message_format);
	vfprintf (stderr, message_format, ap);
	va_end (ap);

	fputc ('\n', stderr);
    }

    if (NULL != usage_msg) {
	fputs ("Usage: ", stderr);

	if (NULL != program_name) {
	    fputs (program_name, stderr);
	    fputc (' ', stderr);
	}

	fputs (usage_msg, stderr);
	fputc ('\n', stderr);
    }

    exit (exitcode);
}


static int parse_args (int argc, const char** argv)
{
    static const char* usage_msg = "etree_name [metadata_file]";

    _opt_program_name = argv[0];


    if (argc < 2) {
	usage (_opt_program_name, usage_msg, 254,
	       "No etree specified in the command line arguments");
	return -1;
    }

    _opt_etreefile = argv[1];

    if (argc > 2) {
	_opt_metafile = argv[2];
    }

    return 0;
}


static int ju_create_temp_file (const char* basename, int keep)
{
    int   fd = -1;
    char* buf;
    long  buf_len;

    buf_len = (basename != NULL) 
	? pathconf (basename, _PC_PATH_MAX)
	: pathconf ("/tmp", _PC_PATH_MAX);

    if (buf_len > 0) {
	buf = (char*)malloc(buf_len+1);

	if (buf != NULL) {
	    buf[buf_len]  = 0;

	    if (basename != NULL) {
		if (buf_len >= strlen (basename) + 6) {
		    strncpy (buf, basename, buf_len);
		    strcat (buf, "XXXXXX");
		    fd = mkstemp(buf);
		}
	    }

	    if (fd < 0) {
		/* try again, this time in /tmp */
		strncpy(buf, "/tmp/XXXXXX", buf_len);
		fd = mkstemp(buf);
	    }

	    if (!keep && fd >= 0) {
		unlink(buf);	    /* remove it from the file system */
	    }

	    free (buf);
	}
    }

    return fd;
}



/**
 * nio_read_all
 *
 * try to read as many bytes as specified in len, unless an error or EOF
 * is found.
 *
 * @return number of bytes read or -1 on error.  If EOF is found before
 *	reading len bytes, then a short count is returned.
 *
 * Note: this function tries to recover from interrupted calls.
 */
int fd_readn (int fd, void* buf, size_t len)
{
    int   rd_left;
    int   rd_count;
    int	  ok;
    char* ptr = (char*)buf;

    rd_count = 1;
    rd_left  = len;
    ok       = 1;

    while (rd_left > 0 && rd_count > 0) {
	rd_count = read (fd,  ptr, rd_left);

	if (rd_count < 0 && EINTR != errno) {
	    /* recover from interrupted syscall, if the error was not EINTR,
	     * then abort the call
	     * else simply ignore EINTR
	     */
	    rd_count = 1;
	    continue;
	} else {		  /* (rd_count >= 0): some bytes or EOF ? */
	    ptr     += rd_count;  /* update counters and pointers */
	    rd_left -= rd_count;
	}
    }

    return rd_count >= 0 ? len - rd_left : -1;
}


static int fd_read_fully (int fd, void* buf, size_t len)
{
    int ret = fd_readn (fd, buf, len);

    return ret == len ? 0 : -1;
}


/**
 * nio_read_all
 *
 * try to read as many bytes as specified in len, unless an error or EOF
 * is found.
 *
 * @return number of bytes read or -1 on error.  If EOF is found before
 *	reading len bytes, then a short count is returned.
 *
 * Note: this function tries to recover from interrupted calls.
 */
int fd_writen (int fd, const void* buf, size_t len)
{
    int   left;
    int   count;
    int	ok;
    char* ptr = (char*)buf;

    count = 1;
    left  = len;
    ok    = 1;

    while (left > 0 && count > 0) {
	count = write(fd,  ptr, left);

	if (count < 0 && EINTR != errno) {
	    /* recover from interrupted syscall, if the error was not EINTR,
	     * then abort the call
	     * else simply ignore EINTR
	 */
	    count = 1;
	    continue;
	} else {		/* (rd_count >= 0): some bytes or EOF ? */
	    ptr  += count;	/* update counters and pointers */
	    left -= count;
	}
    }

    return count >= 0 && left >= 0 ? len - left : -1;
}


int fd_write_fully (int fd, const void* buf, size_t len)
{
    int ret = fd_writen (fd, buf, len);

    return (ret == len) ? 0 : -1;
}

#ifndef  LLONG_MAX
#  define LLONG_MAX	9223372036854775807LL
#endif

/**
 * copy data from one file descriptor to another.
 * the idea is to simply copy from the source fd to the destination fd one
 * buffer at a time.
 *
 * @param src_fd  source file descriptor
 * @param dest_fd destination file descriptor
 * @param length  amount of data to copy, (off_t) -1 means until EOF.
 *
 * @return
 *	0  if length bytes were copied,
 *	1  if EOF on the source fd was reached.
 *	<0 on error.
 *	-1 general error (very likely IO error).
 *	-2 not enough memory.
 */
static int fd_copy (int src_fd, int dest_fd, off_t length)
{
    int		ret = -1;
    size_t	buf_len = 0;
    void*	buf;
    int		read_len;
    struct stat	stat_buf;

    /* get a reasonable buffer size */
    if (fstat (src_fd, &stat_buf) == 0) {
	buf_len = stat_buf.st_blksize;
    }

    if (fstat (dest_fd, &stat_buf) == 0 && stat_buf.st_blksize > buf_len) {
	buf_len = stat_buf.st_blksize;
    }

    if (buf_len == 0) {
	buf_len = getpagesize();
    }

    buf = malloc (buf_len);

    if (buf == NULL) {
	return -2;
    } 

    ret = 0;

    if (length < 0) {
	length = (_FILE_OFFSET_BITS == 64) ? LLONG_MAX : INT_MAX;
    }

    /* read buf_len bytes at a time */
    while (ret == 0 && length > 0) {
	read_len = fd_readn (src_fd, buf,
			     length > buf_len ? buf_len : length);

	if (read_len > 0) {
	    /* write the buffer */
	    if (fd_write_fully (dest_fd, buf, read_len) == 0) {
		length -= read_len;
	    } else {
		ret = -1;
		break;
	    }
	} else {
	    /* check for errors */
	    ret = (read_len == 0) ? 1 : -1;
	    break;
	}
    }

    free (buf);

    return ret;
}


static int copy_2_tmpfile (int input_fd, const char* basename)
{
    int ret;
    int fd;

    /* copy the content to a temporary file */
    fd = ju_create_temp_file (basename, 0);

    if (fd < 0) {
	perror ("Creating temporary metadata file");
	return -1;
    }

    ret = fd_copy (input_fd, fd, -1);

    if (ret < 0) {
	perror ("Failed to copy medatadata to temporary file");
	close (fd);
	fd = -1;
    }

    return fd;
}


static off_t ju_fd_get_file_size (int fd)
{
    struct stat	stat_buf;

    if (fstat (fd, &stat_buf) == 0) {
	return stat_buf.st_size;
    }

    return -1;
}



static int set_app_meta_from_file (etree_t* ep, int fd)
{
    int   ret      = -1;
    char* app_meta = NULL;
    off_t metasize;
    
    assert (ep);
    assert (fd >= 0);

    metasize = ju_fd_get_file_size (fd);

    if (metasize <= 0) {
	return -1;
    }

    app_meta = (char*)malloc (metasize);

    if (app_meta == NULL) {
	fprintf (stderr, "Memory allocation for application metadata failed"
		 ", metadata size = %lld\n", metasize);
	return -1;
    }

    if (lseek (fd, 0, SEEK_SET) == (off_t)-1) {
	perror ("set_app_meta_from_file() fseek failed");
	goto cleanup;
    }

    /* read app meta */
    ret = fd_read_fully (fd, app_meta, metasize);

    if (ret != 0) {
	perror ("ERROR reading metadata file");
	goto cleanup;
    }

    /* set etree app meta */
    ret = etree_setappmeta (ep, app_meta);

    if (ret < 0) {
	etree_error_t etree_error = etree_errno (ep);

	fputs ("ERROR setting etree application metadata\n", stderr);
	fputs (etree_strerror (etree_error), stderr);
	fputc ('\n', stderr);
	goto cleanup;
    }

 cleanup:
    free (app_meta);

    return ret;
}


int main (int argc, const char** argv)
{
    int	     ret = 255;
    int	     fd;
    etree_t* ep;

    ret = parse_args (argc, argv);

    if (ret != 0) {
	return 254;
    }

    /* is the specified etree valid? */
    ep = etree_open (_opt_etreefile, O_RDWR, 0, 0, 0);

    if (ep == NULL) {
	fprintf (stderr, "ERROR: could not open etree '%s'\n", _opt_etreefile);
	return 2;
    }

    if (NULL == _opt_metafile) {
	//	fprintf (stderr, "reading from stdin\n");
	fd = copy_2_tmpfile (0, _opt_etreefile);
	close (0);
    }

    else {
	close (0);
	fd = open (_opt_metafile, O_RDONLY);

	/* something failed */
	if (fd < 0) {
	    perror (_opt_metafile);
	    ret = -1;
	}
    }

    if (fd >= 0) {
	ret = set_app_meta_from_file (ep, fd);
	close (fd);
    }

    etree_close (ep);

    return (ret == 0) ? 0 : 1;
}
