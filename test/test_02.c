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

	axl_list_destroy (list);
	
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
	return 0;
}
