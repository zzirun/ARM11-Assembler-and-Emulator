#include "types.h"

char* pepper(char* str);

long hash(char* to_hash);

bool check_password(char* pw, long hash_pw);

bool check_id(FILE* fp, char* id, long* password, FILE *f);

char* register_new(FILE* fp, char* id);

char* login(FILE *f);