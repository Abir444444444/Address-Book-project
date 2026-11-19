#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contacts.txt", "w");

    if(fp == NULL)
    {
        printf("File opening failed\n");
        return;
    }

    fprintf(fp, "%d\n", addressBook->contactCount);

    for(int i = 0; i < addressBook->contactCount; i++){
        fprintf(fp, "%s %s %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }

    fclose(fp);
}
void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contacts.txt", "r");

    if(fp == NULL)
    {
        printf("File opening failed\n");
        return;
    }

    fscanf(fp, "%d", &addressBook->contactCount);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(fp, "%s %s %s",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }

    fclose(fp);
}