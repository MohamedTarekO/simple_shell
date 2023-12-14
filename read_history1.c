#include "shell.h"
/**
 * close_and_free - closes file descriptor and frees memory
 * @fd: file descriptor
 * @buf: buffer to free
 * @retval: value to return
 *
 * Return: retval
 */
int close_and_free(int fd, char *buf, int retval)
{
	close(fd);
	free(buf);
	return (retval);
}

/**
 * free_and_return - frees memory and returns a value
 * @ptr: pointer to free
 * @retval: value to return
 *
 * Return: retval
 */
char *free_and_return(char *ptr, char *retval)
{
	free(ptr);
	return (retval);
}

