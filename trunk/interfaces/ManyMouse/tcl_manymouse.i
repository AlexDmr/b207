/* File : example.i */
%module tcl_multimouse

%{
#include "manymouse.h"
%}

/* Let's just grab the original header file here */
%include "manymouse.h"

