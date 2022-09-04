#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "Generals.h"

bool Equality_double (double num1, double num2){
    is_error (isfinite (num1));
    is_error (isfinite (num2));

    return fabs (num1-num2) < Eps;
}

bool Is_zero (double num){
    is_error (isfinite (num));

    return Equality_double (num, 0);
}

double Fix_zero (double num){
    is_error (isfinite (num));

    if (Is_zero (num))
        return 0.0;
    return num;
}

void Print_colour (char const colour[], char const *str, ...){
    printf ("%s", colour);

    va_list arg_ptr;

    va_start (arg_ptr, str);
    vprintf (str, arg_ptr);
    va_end (arg_ptr);

    printf ("%s", RESET);
}

FILE *Open_file (const char *name_file, const char *mode){
    FILE *fp = fopen (name_file, mode);
    if (!fp){
        fprintf (stderr, "Could't open file %s with mode: %s\n", name_file, mode);
        
        abort ();
    }

    return fp;
}

int Parsing (int argc, char *argv[], Options *flags){
    while (--argc > 0){
        ++argv;
		
		if ((*argv)[0] == '-'){
			char ch = 0;
			while ((ch = *++argv[0])){
				switch (ch){
					case 'r':
						flags->Read_on_file = true;
						break;

					case 'w':
						flags->Write_on_file = true;
						break;
					
					default:
						break;
				}
			}

		}
    }

	return 0;
}

int My_swap (void *obj1, void *obj2, size_t size_type){                         
    assert (obj1 != NULL && "obj1 is NULL");
	assert (obj2 != NULL && "obj2 is NULL");


	char* _obj1  = (char*) obj1;
	char* _obj2  = (char*) obj2;

	while (size_type >= sizeof (int64_t)) {
		int64_t temp       = *(int64_t*) _obj1;
		*(int64_t*) _obj1  = *(int64_t*) _obj2;
		*(int64_t*) _obj2  = temp;

		_obj1 += sizeof (int64_t);
		_obj2 += sizeof (int64_t);

		size_type -= sizeof (int64_t);
	}

	while (size_type >= sizeof (int32_t)) {
		int32_t temp       = *(int32_t*) _obj1;
		*(int32_t*) _obj1  = *(int32_t*) _obj2;
		*(int32_t*) _obj2  = temp;

		_obj1 += sizeof (int32_t);
		_obj2 += sizeof (int32_t);

		size_type -= sizeof (int32_t);
	}

	while (size_type >= sizeof (int16_t)) {
		int16_t temp       = *(int16_t*) _obj1;
		*(int16_t*) _obj1  = *(int16_t*) _obj2;
		*(int16_t*) _obj2  = temp;

		_obj1 += sizeof (int16_t);
		_obj2 += sizeof (int16_t);

		size_type -= sizeof (int16_t);
	}

	while (size_type >= sizeof (int8_t)) {
		int8_t temp       = *(int8_t*) _obj1;
		*(int8_t*) _obj1  = *(int8_t*) _obj2;
		*(int8_t*) _obj2  = temp;

		_obj1 += sizeof (int8_t);
		_obj2 += sizeof (int8_t);

		size_type -= sizeof (int8_t);
	}

	return 0;
}