#include "shell.h"
/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);

	fsize = get_file_size(fd, &st);
	if (fsize < 2)
		return (close_and_free(fd, NULL, 0));

	buf = read_file_content(fd, fsize);
	if (!buf)
		return (close_and_free(fd, NULL, 0));

	close(fd);

	process_history_content(info, buf, fsize);

	return (info->histcount);
}

/**
 * process_history_content - processes history content
 * @info: the parameter struct
 * @buf: buffer containing history content
 * @fsize: size of the buffer
 *
 * Return: histcount on success, 0 otherwise
 */
int process_history_content(info_t *info, char *buf, ssize_t fsize)
{
	int i, last = 0, linecount = 0;

	for (i = 0; i < fsize; i++)
	{
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	}

	if (last != i)
		build_history_list(info, buf + last, linecount++);

	info->histcount = linecount;

	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);

	renumber_history(info);

	return (info->histcount);
}

/**
 * get_file_size - gets the size of a file
 * @fd: file descriptor
 * @st: pointer to struct stat
 *
 * Return: file size on success, -1 on failure
 */
ssize_t get_file_size(int fd, struct stat *st)
{
	if (!fstat(fd, st))
		return (st->st_size);

	return (-1);
}

/**
 * read_file_content - reads content from a file
 * @fd: file descriptor
 * @fsize: size of the file
 *
 * Return: buffer containing file content
 */
char *read_file_content(int fd, ssize_t fsize)
{
	char *buf = malloc(sizeof(char) * (fsize + 1));

	if (!buf)
		return (NULL);

	ssize_t rdlen = read(fd, buf, fsize);

	buf[fsize] = '\0';

	if (rdlen <= 0)
		return (free_and_return(buf, NULL));

	return (buf);
}

