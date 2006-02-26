#include <axl.h>
#include <stdio.h>

/** 
 * @brief Test current libaxl list implementation.
 * 
 * 
 * @return AXL_TRUE if it works properly or AXL_FALSE if not.
 */
bool test_01 () 
{
	axlList * list;

	/* create the list */
	list = axl_list_new (axl_list_equal_string, NULL);
	if (axl_list_length (list) != 0) {
		printf ("Bad length returned by the list\n");
		return AXL_FALSE;
	}
	
	/* add data */
	axl_list_add (list, "test 1");
	if (axl_list_length (list) != 1) {
		printf ("Bad length returned by the list\n");
		return AXL_FALSE;
	}
	
	axl_list_add (list, "test 2");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list\n");
		return AXL_FALSE;
	}

	axl_list_add (list, "test 3");
	if (axl_list_length (list) != 3) {
		printf ("Bad length returned by the list\n");
		return AXL_FALSE;
	}

	/* remove data */
	axl_list_remove (list, "test 1");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return AXL_FALSE;
	}

	axl_list_remove (list, "test 2");
	if (axl_list_length (list) != 1) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return AXL_FALSE;
	}

	axl_list_remove (list, "test 3");
	if (axl_list_length (list) != 0) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return AXL_FALSE;
	}

	/* add data again */
	axl_list_add (list, "test 4");
	axl_list_add (list, "test 5");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return AXL_FALSE;
	}
	
	axl_list_remove (list, "test 1");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return AXL_FALSE;
	}

	if (! axl_list_exists (list, "test 5")) {
		printf ("Exist function have failed\n");
		return AXL_FALSE;
	}

	if (! axl_list_exists (list, "test 4")) {
		printf ("Exist function have failed\n");
		return AXL_FALSE;
	}

	if (axl_list_exists (list, "test 1")) {
		printf ("Exist function have failed\n");
		return AXL_FALSE;
	}

	if (! axl_list_exists_at (list, "test 4", 0)) {
		printf ("\"Exists at\" functionality seems to not work (0)\n");
		return AXL_FALSE;
	}

	if (! axl_list_exists_at (list, "test 5", 1)) {
		printf ("\"Exists at\" functionality seems to not work (1)\n");
		return AXL_FALSE;
	}

	axl_list_free (list);
	
	return AXL_TRUE;
}

/** 
 * @brief Checks current configuration for the stack implementation
 * based on the axlList.
 * 
 * 
 * @return AXL_TRUE if the stack works properly, otherwise AXL_FALSE
 * is returned.
 */
bool test_02 () 
{
	axlStack * stack;
	char     * value;

	/* create the stack */
	stack = axl_stack_new (NULL);

	axl_stack_push (stack, "test 1");
	axl_stack_push (stack, "test 2");
	axl_stack_push (stack, "test 3");

	if (axl_stack_size (stack) != 3) {
		printf ("Wrong stack size expected ..\n");
		return AXL_FALSE;
	}
	
	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 3", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 3");
		return AXL_FALSE;
	}

	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 2", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 2");
		return AXL_FALSE;
	}

	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 1", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 1");
		return AXL_FALSE;
	}

	if (axl_stack_size (stack) != 0) {
		printf ("Wrong stack size before operating..\n");
		return AXL_FALSE;
	}

	if (! axl_stack_is_empty (stack)) {
		printf ("Wrong stack emptyness value received..\n");
		return AXL_FALSE;
	}

	/* destroy the stack */
	axl_stack_destroy (stack);

	return AXL_TRUE;
}

/** 
 *
 * @brief Checks some internal functions that the library provides to
 * manage strings.
 *
 * @return The function must return AXL_TRUE if everything is
 * ok. Otherwise AXL_FALSE is returned.
 */
bool test_03 () 
{
	char  * string;
	char ** result;

	/* check that axl_stream_split works */
	result = axl_stream_split ("value/value1/value3/value4", 1, "/");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions\n");
		return AXL_FALSE;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d)\n", 
			axl_stream_strv_num (result),4);
		return AXL_FALSE;
	}
	
	if (! axl_cmp (result[0], "value")) {
		printf ("Failed to get the first element: (%s != %s)\n", result[0], "value");
		return AXL_FALSE;
	}

	if (! axl_cmp (result[1], "value1")) {
		printf ("Failed to get the second element: (%s != %s)\n", result[1], "value1");
		return AXL_FALSE;
	}

	if (! axl_cmp (result[2], "value3")) {
		printf ("Failed to get the third element (%s != %s)\n", result[2], "value3");
		return AXL_FALSE;
	}

	if (! axl_cmp (result[3], "value4")) {
		printf ("Failed to get the fourth element (%s != %s)\n", result[3], "value4");
		return AXL_FALSE;
	}

	/* release memory used */
	axl_stream_freev (result);

	result = axl_stream_split ("value1, value2/ value3* ", 3, ", ", "/ ", "* ");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions (2)\n");
		return AXL_FALSE;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d) (2)\n", 
			axl_stream_strv_num (result), 4);
		return AXL_FALSE;
	}

	if (! axl_cmp (result[0], "value1")) {
		printf ("Failed to get the second element: (%s != %s)\n", result[0], "value1");
		return AXL_FALSE;
	}

	if (! axl_cmp (result[1], "value2")) {
		printf ("Failed to get the third element (%s != %s)\n", result[1], "value2");
		return AXL_FALSE;
	}

	if (! axl_cmp (result[2], "value3")) {
		printf ("Failed to get the fourth element (%s != %s)\n", result[2], "value3");
		return AXL_FALSE;
	}

	if (! axl_cmp (result[3], "")) {
		printf ("Failed to get the fourth element ('%s' != '%s')\n", result[3], "");
		return AXL_FALSE;
	}

	

	/* release memory used */
	axl_stream_freev (result);

	/* check empty cases */
	result = axl_stream_split ("///", 1, "/");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions (3)\n");
		return AXL_FALSE;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d) (3)\n", 
			axl_stream_strv_num (result), 4);
		return AXL_FALSE;
	}

	/* release memory used */
	axl_stream_freev (result);

	string = axl_strdup ("AxBcA");
	if (! axl_cmp (axl_stream_to_upper (string), "AXBCA")) {
		/* report error found */
		printf ("failed to upper case letters\n");
		return AXL_FALSE;
	}
	axl_free (string);

	string = axl_strdup ("AxBcA");
	if (! axl_cmp (axl_stream_to_lower (string), "axbca")) {
		/* report error found */
		printf ("failed to lower case letters, result found: %s != %s\n", string, "axbca");
		return AXL_FALSE;
	}
	axl_free (string);


	
	return AXL_TRUE;
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

	return 0;
}
