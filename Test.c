#include "Json.c"
#include <stdio.h>

int main() {
    // Create the root object
JsonObject *root = (JsonObject *)CreateJsonObject();

// Add a string
AddToJsonObject(root, (JsonString *)CreateJsonStringNullTerminated("name", true), CreateJsonStringNullTerminated("Nathaniel", false));

// Add a number
AddToJsonObject(root, (JsonString *)CreateJsonStringNullTerminated("age", true), CreateJsonNumber(1824.23));

// Add a boolean
AddToJsonObject(root, (JsonString *)CreateJsonStringNullTerminated("isDeveloper", true), CreateJsonBoolean(true));

// Add a null value
AddToJsonObject(root, (JsonString *)CreateJsonStringNullTerminated("spouse", true), CreateJsonNull());

// Add an array
JsonArray *skills = (JsonArray *)CreateJsonArray();
AddToJsonArray(skills, CreateJsonStringNullTerminated("C", false));
AddToJsonArray(skills, CreateJsonStringNullTerminated("C++", false));
AddToJsonArray(skills, CreateJsonStringNullTerminated("Lua", false));
AddToJsonArray(skills, CreateJsonStringNullTerminated("Brainfuck", false));
AddToJsonObject(root, (JsonString *)CreateJsonStringNullTerminated("skills", true), (JsonData *)skills);

// Add a nested object
JsonObject *address = (JsonObject *)CreateJsonObject();
AddToJsonObject(address, (JsonString *)CreateJsonStringNullTerminated("city", true), CreateJsonStringNullTerminated("New York", false));
AddToJsonObject(address, (JsonString *)CreateJsonStringNullTerminated("zip", true), CreateJsonNumber(10001));
AddToJsonObject(root, (JsonString *)CreateJsonStringNullTerminated("address", true), (JsonData *)address);
    FILE *file = fopen("./input.json", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }
    fseek(file, 0, SEEK_END); // Move the file pointer to the end of the file
    long size = ftell(file);   // Get the current position, which is the file size
    fseek(file, 0, SEEK_SET);  // Move the file pointer back to the beginning
    char *buffer = malloc(size + 1); // Allocate buffer for file content
    if (!buffer) {
        perror("Failed to allocate memory");
        fclose(file);
        return 1;
    }
    fread(buffer, 1, size, file); // Read the file content into the buffer
    buffer[size] = '\0'; // Null-terminate the buffer
    fclose(file);

    char *minimized = JsonMinimizeString(buffer);
    free(buffer);
    if (!minimized) {
        perror("Failed to minimize JSON string");
        return 1;
    }
    printf("Minimized JSON:%s\n", minimized);
    // do nothing with the inputted json because there's nothing in the parser yet.

    char *jsonString = PPrintJsonData((JsonData *)root, 0);
    
    FILE *fp;
    fp = fopen("./output.json", "w"); // Open "output.json" for writing
    if (fp == NULL) {
        // Handle error: file could not be opened
        perror("Error opening file");
        return 1; // Or some other error handling
    }
    if (jsonString) {
        printf("%s\n", jsonString);
        fprintf(fp, "%s\n", jsonString); // Write the JSON string to the file
        fclose(fp); // Close the file
        free(jsonString);
    }

    /* Free the JSON structure we created */
    FreeJsonData((JsonData *)root);

    return 0;
}