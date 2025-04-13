#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_VARS 100

typedef enum { INT_TYPE, STRING_TYPE } VarType;

typedef union {
    int int_value;
    char string_value[100];
} VarValue;

typedef struct {
    char name[50];
    VarType type;
    VarValue data;
} Variable;

Variable variables[MAX_VARS];
int var_count = 0;

void trim(char *str) {
    int i, start = 0, end = strlen(str) - 1;
    while (isspace(str[start])) start++;
    while (end > start && isspace(str[end])) end--;
    for (i = start; i <= end; i++) str[i - start] = str[i];
    str[i - start] = '\0';
}

int find_variable_index(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int get_int_value(const char *name) {
    int idx = find_variable_index(name);
    if (idx != -1 && variables[idx].type == INT_TYPE) {
        return variables[idx].data.int_value;
    }
    printf("Error: Undefined variable '%s'\n", name);
    return 0;
}

void set_variable(const char *name, const char *value_str) {
    int idx = find_variable_index(name);

    Variable v;
    strcpy(v.name, name);

    // Check if it's a string (surrounded by quotes)
    if (value_str[0] == '"' && value_str[strlen(value_str) - 1] == '"') {
        v.type = STRING_TYPE;
        strncpy(v.data.string_value, value_str + 1, strlen(value_str) - 2);
        v.data.string_value[strlen(value_str) - 2] = '\0';
    }
    // Check if it's a number
    else if (isdigit(value_str[0]) || (value_str[0] == '-' && isdigit(value_str[1]))) {
        v.type = INT_TYPE;
        v.data.int_value = atoi(value_str);
    }
    // Otherwise, assume it's another variable
    else {
        int var_idx = find_variable_index(value_str);
        if (var_idx != -1) {
            v.type = variables[var_idx].type;
            v.data = variables[var_idx].data;
        } else {
            printf("Error: Undefined variable '%s'\n", value_str);
            return;
        }
    }

    // Store variable
    if (idx != -1) {
        variables[idx] = v;
    } else if (var_count < MAX_VARS) {
        variables[var_count++] = v;
    }
}


void execute_line(char *line) {
    trim(line);

    if (strncmp(line, "print(", 6) == 0 && line[strlen(line) - 1] == ')') {
        char content[100];
        sscanf(line + 6, "%[^)]", content);
        trim(content);

        if (content[0] == '"' && content[strlen(content) - 1] == '"') {
            content[strlen(content) - 1] = 0;
            printf("%s\n", content + 1);
            return;
        }

        char var1[50], var2[50], op;
        if (sscanf(content, "%[^+*/-] %c %s", var1, &op, var2) == 3) {
            trim(var1);
            trim(var2);
            int val1 = get_int_value(var1);
            int val2 = get_int_value(var2);
            int result = 0;

            if (op == '+') result = val1 + val2;
            else if (op == '-') result = val1 - val2;
            else if (op == '*') result = val1 * val2;
            else if (op == '/') result = (val2 != 0) ? val1 / val2 : 0;
            else {
                printf("Error: Unsupported operator '%c'\n", op);
                return;
            }

            printf("%d\n", result);
            return;
        }

        int idx = find_variable_index(content);
        if (idx != -1) {
            if (variables[idx].type == INT_TYPE) {
                printf("%d\n", variables[idx].data.int_value);
            } else if (variables[idx].type == STRING_TYPE) {
                printf("%s\n", variables[idx].data.string_value);
            }
        } else {
            printf("Error: Undefined variable '%s'\n", content);
        }
        return;
    }

    // Assignment handling: x=5, x = "hi"
    char *equals = strchr(line, '=');
    if (equals) {
        char var_name[50], value_str[100];
        size_t name_len = equals - line;
        strncpy(var_name, line, name_len);
        var_name[name_len] = '\0';
        strcpy(value_str, equals + 1);
        trim(var_name);
        trim(value_str);
        set_variable(var_name, value_str);
        return;
    }

    printf("Syntax Error: %s\n", line);
}

int main() {
    char input[256], code[1024] = "";
    printf("Simple Python Interpreter in C\n");

    while (1) {
        printf(">>> ");
        fgets(input, sizeof(input), stdin);
        trim(input);

        if (strcmp(input, "RUN") == 0) {
            printf("\n--- Running Code ---\n");
            char *line = strtok(code, "\n");
            while (line != NULL) {
                execute_line(line);
                line = strtok(NULL, "\n");
            }
            printf("--- Execution Finished ---\n\n");
            code[0] = '\0';
        } else if (strcmp(input, "EXIT") == 0) {
            break;
        } else {
            strcat(code, input);
            strcat(code, "\n");
        }
    }

    return 0;
}
