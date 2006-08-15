#include <axl.h>
#include <stdio.h>

/** 
 * @brief Test current libaxl list implementation.
 * 
 * 
 * @return true if it works properly or false if not.
 */
bool test_01 () 
{
	axlList * list;

	/* create the list */
	list = axl_list_new (axl_list_equal_string, NULL);
	if (axl_list_length (list) != 0) {
		printf ("Bad length returned by the list (%d != 0)\n",
			axl_list_length (list));
		return false;
	}
	
	/* add data */
	axl_list_add (list, "test 1");
	if (axl_list_length (list) != 1) {
		printf ("Bad length returned by the list (%d != 1)\n",
			axl_list_length (list));
		return false;
	}
	
	axl_list_add (list, "test 2");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list (%d != 2)\n",
			axl_list_length (list));
		return false;
	}

	axl_list_add (list, "test 3");
	if (axl_list_length (list) != 3) {
		printf ("Bad length returned by the list (%d != 3)\n",
			axl_list_length (list));
		return false;
	}

	/* remove data */
	axl_list_remove (list, "test 1");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work (%d != 2)\n",
			axl_list_length (list));
		return false;
	}

	axl_list_remove (list, "test 2");
	if (axl_list_length (list) != 1) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return false;
	}

	axl_list_remove (list, "test 3");
	if (axl_list_length (list) != 0) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return false;
	}

	/* add data again */
	axl_list_add (list, "test 4");
	axl_list_add (list, "test 5");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return false;
	}
	
	axl_list_remove (list, "test 1");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return false;
	}

	if (! axl_list_exists (list, "test 5")) {
		printf ("Exist function have failed\n");
		return false;
	}

	if (! axl_list_exists (list, "test 4")) {
		printf ("Exist function have failed\n");
		return false;
	}
	
	if (axl_list_exists (list, "test 1")) {
		printf ("Exist function have failed\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 4", 0)) {
		printf ("\"Exists at\" functionality seems to not work (0)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 5", 1)) {
		printf ("\"Exists at\" functionality seems to not work (1)\n");
		return false;
	}

	axl_list_free (list);

	/* create a new list */
	list = axl_list_new (axl_list_equal_string, NULL);

	axl_list_add (list, "test 1");
	axl_list_add (list, "test 3");
	axl_list_add (list, "test 5");

	axl_list_add_at (list, "test 0", 0);
	axl_list_add_at (list, "test 2", 2);
	axl_list_add_at (list, "test 4", 4);
	axl_list_add_at (list, "test 6", 6);

	if (! axl_list_exists_at (list, "test 0", 0)) {
		printf ("\"Exists at\" functionality seems to not work (0)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 1", 1)) {
		printf ("\"Exists at\" functionality seems to not work (1)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 2", 2)) {
		printf ("\"Exists at\" functionality seems to not work (2)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 3", 3)) {
		printf ("\"Exists at\" functionality seems to not work (3)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 4", 4)) {
		printf ("\"Exists at\" functionality seems to not work (4)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 5", 5)) {
		printf ("\"Exists at\" functionality seems to not work (5)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 6", 6)) {
		printf ("\"Exists at\" functionality seems to not work (6)\n");
		return false;
	}

	/* free the list */
	axl_list_free (list);
	
	return true;
}

bool test_02_foreach (axlPointer stack_data, 
		      axlPointer user_data, 
		      axlPointer user_data2)
{
	int * iterator = user_data;

	if ((*iterator == 0) && axl_cmp ((char*) stack_data, "test 3")) {
		(*iterator)++;
		return false;
	} else if ((*iterator == 1) && axl_cmp ((char*) stack_data, "test 2")) {
		(*iterator)++;
		return false;
	} else if ((*iterator == 2) && axl_cmp ((char*) stack_data, "test 1")) {
		(*iterator)++;
		return false;
	}
	
	/* return true to stop operations */
	return true;
}

/** 
 * @brief Checks current configuration for the stack implementation
 * based on the axlList.
 * 
 * 
 * @return true if the stack works properly, otherwise false
 * is returned.
 */
bool test_02 () 
{
	axlStack * stack;
	char     * value;
	int        iterator = 0;

	/* create the stack */
	stack = axl_stack_new (NULL);

	axl_stack_push (stack, "test 1");
	axl_stack_push (stack, "test 2");
	axl_stack_push (stack, "test 3");

	/* check foreach function */
	axl_stack_foreach (stack, test_02_foreach, &iterator, NULL);

	if (iterator != 3) {
		printf ("Wrong value expected while using the foreach function\n");
		return false;
	}

	if (axl_stack_size (stack) != 3) {
		printf ("Wrong stack size expected ..\n");
		return false;
	}
	
	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 3", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 3");
		return false;
	}

	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 2", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 2");
		return false;
	}

	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 1", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 1");
		return false;
	}

	if (axl_stack_size (stack) != 0) {
		printf ("Wrong stack size before operating..\n");
		return false;
	}

	if (! axl_stack_is_empty (stack)) {
		printf ("Wrong stack emptyness value received..\n");
		return false;
	}

	/* destroy the stack */
	axl_stack_free (stack);

	return true;
}

/** 
 *
 * @brief Checks some internal functions that the library provides to
 * manage strings.
 *
 * @return The function must return true if everything is
 * ok. Otherwise false is returned.
 */
bool test_03 () 
{
	char  * string;
	char ** result;
	int     trimmed;

	/* check that axl_stream_split works */
	result = axl_stream_split ("value/value1/value3/value4", 1, "/");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions\n");
		return false;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d)\n", 
			axl_stream_strv_num (result),4);
		return false;
	}
	
	if (! axl_cmp (result[0], "value")) {
		printf ("Failed to get the first element: (%s != %s)\n", result[0], "value");
		return false;
	}

	if (! axl_cmp (result[1], "value1")) {
		printf ("Failed to get the second element: (%s != %s)\n", result[1], "value1");
		return false;
	}

	if (! axl_cmp (result[2], "value3")) {
		printf ("Failed to get the third element (%s != %s)\n", result[2], "value3");
		return false;
	}

	if (! axl_cmp (result[3], "value4")) {
		printf ("Failed to get the fourth element (%s != %s)\n", result[3], "value4");
		return false;
	}

	/* release memory used */
	axl_stream_freev (result);

	result = axl_stream_split ("value1, value2/ value3* ", 3, ", ", "/ ", "* ");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions (2)\n");
		return false;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d) (2)\n", 
			axl_stream_strv_num (result), 4);
		return false;
	}

	if (! axl_cmp (result[0], "value1")) {
		printf ("Failed to get the second element: (%s != %s)\n", result[0], "value1");
		return false;
	}

	if (! axl_cmp (result[1], "value2")) {
		printf ("Failed to get the third element (%s != %s)\n", result[1], "value2");
		return false;
	}

	if (! axl_cmp (result[2], "value3")) {
		printf ("Failed to get the fourth element (%s != %s)\n", result[2], "value3");
		return false;
	}

	if (! axl_cmp (result[3], "")) {
		printf ("Failed to get the fourth element ('%s' != '%s')\n", result[3], "");
		return false;
	}

	

	/* release memory used */
	axl_stream_freev (result);

	/* check empty cases */
	result = axl_stream_split ("///", 1, "/");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions (3)\n");
		return false;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d) (3)\n", 
			axl_stream_strv_num (result), 4);
		return false;
	}

	/* release memory used */
	axl_stream_freev (result);

	string = axl_strdup ("AxBcA");
	if (! axl_cmp (axl_stream_to_upper (string), "AXBCA")) {
		/* report error found */
		printf ("failed to upper case letters\n");
		return false;
	}
	axl_free (string);

	string = axl_strdup ("AxBcA");
	if (! axl_cmp (axl_stream_to_lower (string), "axbca")) {
		/* report error found */
		printf ("failed to lower case letters, result found: %s != %s\n", string, "axbca");
		return false;
	}
	axl_free (string);


	string = axl_strdup ("\t \n   \r  value  \n \r \t \t  ");
	axl_stream_trim (string);

	if (! axl_cmp (string, "value")) {
		printf ("failed to trim the string received, expected '%s' == '%s'\n", 
			string, "value");
		return false;
	}

	/* release memory */
	axl_free (string);

	string = axl_strdup ("   value    ");
	axl_stream_trim_with_size (string, &trimmed);
	if (trimmed != 7) {
		printf ("failed, expected to find a trimmed size of 7 but found %d (%s)\n", 
			trimmed, string);
		return false;
	}

	/* release the memory */
	axl_free (string);
	
	return true;
}

/** 
 * @brief Intensive axl list implementation.
 */
bool test_04 () {
	int       iterator = 0;
	axlList * list;

	/* create the list */
	list = axl_list_new (axl_list_always_return_1, NULL);

	/* add items */
	while (iterator < 10000) {

		/* add integers */
		axl_list_add (list, &iterator);

		/* update the iterator */
		iterator++;
	}

	/* release the list */
	axl_list_free (list);
	
	/* true */
	return true;
}

/** 
 * @brief Perform some operations using some interfaces provided by
 * the libaxl library.
 * 
 */
int main (int argc, char ** argv) 
{
	if (test_01 ()) {
		printf ("LibAxl list implementation [   OK   ]\n");
	}else {
		printf ("LibAxl list implementation [ FAILED ]\n");
	}

	if (test_02 ()) {
		printf ("LibAxl FIFO implementation [   OK   ]\n");
	}else {
		printf ("LibAxl list implementation [ FAILED ]\n");
	}

	if (test_03 ()) {
		printf ("LibAxl string functions    [   OK   ]\n");
	}else {
		printf ("LibAxl string functions    [ FAILED ]\n");
	}

	if (test_04 ()) {
		printf ("LibAxl list implementation (II) [   OK   ]\n");
	}else {
		printf ("LibAxl list implementation (II) [ FAILED ]\n");
	}

	return 0;
}
