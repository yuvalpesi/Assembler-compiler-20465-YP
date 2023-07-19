
/**
 * Retrieves the input files to be compiled and processes them one by one.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 if the files are processed successfully, or an appropriate error code.
 */
int getFile(int argc,char **argv);

/**
 * Sets up the compilation process for the input assembly file.
 * Performs preprocessing, first pass, and second pass to generate the object file.
 *
 * @param argv Pointer to a string representing the name of the input assembly file.
 */
void setFile(char *argv);

