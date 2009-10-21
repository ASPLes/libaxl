#!/usr/bin/perl

# path to command
$axl_knife = "../knife/axl-knife";
chomp ($diff = `which diff`);

sub run_cmd {
    # get string
    my $string = $_[0];

    $result = system ($string);
    if ($result != 0) {
	print ("ERROR: found error code ($result) while running $string..\n");
	exit (-1);
    }
    return $result;
}

sub test_01 () {

    ### test ####
    # call to produce knife representation check with reference
    run_cmd ("$axl_knife -i knife.test_01.xml -o knife.test_01.xml.tmp -e");

    # call to get differences
    run_cmd ("$diff knife.test_01.xml.tmp knife.test_01.ref");

    ### test ####
    # call to produce knife representation check with reference
    run_cmd ("$axl_knife -i knife.test_01.b.xml -o knife.test_01.xml.b.tmp -e");

    # call to get differences
    run_cmd ("$diff knife.test_01.xml.b.tmp knife.test_01.b.ref");

    ### test ####
    # call to produce knife representation check with reference
    run_cmd ("$axl_knife -i knife.test_01.c.xml -o knife.test_01.xml.c.tmp -e");

    # call to get differences
    run_cmd ("$diff knife.test_01.xml.c.tmp knife.test_01.c.ref");

    ### test ####
    # call to produce knife representation check with reference
    run_cmd ("$axl_knife -i knife.test_01.d.xml -o knife.test_01.xml.d.tmp -e");

    # call to get differences
    run_cmd ("$diff knife.test_01.xml.d.tmp knife.test_01.d.ref");

    return 1;
}


########
# MAIN #
########

# check if the command is defined
if (! -e $axl_knife) {
    print ("ERROR: unable to find axl-knife tool at: $axl_knife..\n");
    exit (-1);
}

if (! test_01 ()) {
    print ("Test 01: check htmlize function [ FAILED ]\n");
    exit (-1);
} else {
    print ("Test 01: check htmlize function [   OK   ]\n");
}

print ("All tests ok!\n");
