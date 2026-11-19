#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "contact.h"
#include "file.h"

int SearchChoice()
{
    int choice;

    while(1){
        printf("\nSearch By\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("Enter choice: ");

        if(scanf("%d", &choice) != 1)
        {
            while(getchar() != '\n');

            printf("Invalid choice\n");
            continue;
        }

        if(choice >= 1 && choice <= 3)
        {
            return choice;
        }

        printf("Invalid choice\n");
    }
}

void SearchInput(int choice, char *search)
{
    if(choice == 1)
    {
        printf("Enter name: ");
    }
    else if(choice == 2)
    {
        printf("Enter phone: ");
    }
    else
    {
        printf("Enter email: ");
    }

    scanf(" %99[^\n]", search);
}

void displayContact(Contact *contact)
{
    printf("------------------------------------------------------------\n");
    printf("Name  : %s\n", contact->name);
    printf("Phone : %s\n", contact->phone);
    printf("Email : %s\n", contact->email);
    printf("------------------------------------------------------------\n");
}

int name_validate(AddressBook *addressBook, char *name)
{
    for(int i = 0; name[i] != '\0'; i++)
    {
        if(!(isalpha((unsigned char)name[i]) ||
             isspace((unsigned char)name[i]) ||
             name[i] == '.'))
        {
            return 0;
        }
    }

    return 1;
}

int phoneno_validate(AddressBook *addressBook, char *phone)
{
    if(strlen(phone) != 10){
        return 0;
    }

    for(int i = 0; phone[i] != '\0'; i++){
        if(!isdigit((unsigned char)phone[i])){
            return 0;
        }
    }

    for(int i = 0; i < addressBook->contactCount; i++){
        if(strcmp(addressBook->contacts[i].phone, phone) == 0){
            return 0;
        }
    }

    return 1;
}

int email_validate(AddressBook *addressBook, char *email){
    int i = 0;

    while(email[i] != '\0'){
        if(!(islower((unsigned char)email[i]) || isdigit((unsigned char)email[i]) ||email[i] == '@' ||email[i] == '.')){
            return 0;
        }

        i++;
    }

    for(int a = 0; a < addressBook->contactCount; a++){
        if(strcmp(addressBook->contacts[a].email, email) == 0){
            return 0;
        }
    }

    char *at = strchr(email, '@');

    if(at == NULL || strcmp(at, "@gmail.com") != 0){
        return 0;
    }

    return 1;
}

int searchContacts(AddressBook *addressBook,char *search,int choice,int *matchedIndex){
    int count = 0;

    for(int i = 0; i < addressBook->contactCount; i++){
        if(choice == 1 &&strcmp(addressBook->contacts[i].name, search) == 0){
            matchedIndex[count] = i;
            count++;
        }
        else if(choice == 2 && strcmp(addressBook->contacts[i].phone, search) == 0){
            matchedIndex[count] = i;
            count++;
        }
        else if(choice == 3 &&
                strcmp(addressBook->contacts[i].email, search) == 0){
            matchedIndex[count] = i;
            count++;
        }
    }

    return count;
}

int ContactIndex(AddressBook *addressBook,char *search,int choice){
    int matchedIndex[100];
    int count;

    count = searchContacts(addressBook,search,choice,matchedIndex);

    if(count == 0){
        return -1;
    }

    if(count == 1){
        return matchedIndex[0];
    }

    printf("Multiple contacts found:\n");

    for(int i = 0; i < count; i++){
        int idx = matchedIndex[i];

        printf("%d.\n", i + 1);
        displayContact(&addressBook->contacts[idx]);
    }

    int select;

    while(1)
    {
        printf("Select contact : ");

        if(scanf("%d", &select) != 1)
        {
            printf("Invalid choice\n");

            while(getchar() != '\n');

            continue;
        }

        if(select >= 1 && select <= count){
            break;
        }

        printf("Invalid selection\n");
    }

    return matchedIndex[select - 1];
}

int searchAndSelectContact(AddressBook *addressBook){

   int choice;
    int index;

    char search[100];

    choice = SearchChoice();

    SearchInput(choice, search);

    index = ContactIndex(addressBook,search,choice);

    if(index == -1){
        printf("Contact not found\n");
        return -1;
    }

    return index;
}

void createContact(AddressBook *addressBook){
    int index = addressBook->contactCount;
    int flag;

    do{
        printf("Enter the name: ");
        scanf(" %99[^\n]", addressBook->contacts[index].name);

        flag = name_validate(addressBook,
                             addressBook->contacts[index].name);

        if(!flag){
            printf("Invalid Name\n");
        }

    } while(!flag);

    do{
        printf("Enter Phone no: ");
        scanf(" %99[^\n]", addressBook->contacts[index].phone);

        flag = phoneno_validate(addressBook,addressBook->contacts[index].phone);

        if(!flag){
            printf("Invalid Phone No\n");
        }

    } while(!flag);

    do{
        printf("Enter Email: ");
        scanf(" %99[^\n]", addressBook->contacts[index].email);

        flag = email_validate(addressBook,addressBook->contacts[index].email);

        if(!flag)
        {
            printf("Invalid Email\n");
        }

    } while(!flag);

    addressBook->contactCount++;
}

void searchContact(AddressBook *addressBook)
{
    char search[100];

    int choice;
    int matchedIndex[100];
    int count;

    printf("-------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\tENTERED INTO SEARCH CONTACT\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    choice = SearchChoice();

    SearchInput(choice, search);

    count = searchContacts(addressBook,search,choice,matchedIndex);

    if(count == 0){
        printf("Contact not found\n");
        return;
    }

    for(int i = 0; i < count; i++){
        displayContact(&addressBook->contacts[matchedIndex[i]]);
    }
}

void editContact(AddressBook *addressBook){
    char edit[100];
    int choice;
    int temp;

    printf("-------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\tENTERED INTO EDIT CONTACT\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    temp = searchAndSelectContact(addressBook);

    if(temp < 0){
        return;
    }

    printf("\nWhat do you want to edit?\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter choice: ");

    if(scanf("%d", &choice) != 1){
        while(getchar() != '\n');

        printf("Invalid choice\n");
        return;
    }

    switch(choice){
        case 1:

        printf("Enter new name: ");
        scanf(" %99[^\n]", edit);

        if(name_validate(addressBook, edit)){
                strcpy(addressBook->contacts[temp].name, edit);
        }
        else{
                printf("Invalid name\n");
        }
        break;
        case 2:
        printf("Enter new phone: ");
         scanf(" %99[^\n]", edit);

        if(phoneno_validate(addressBook, edit)){
                strcpy(addressBook->contacts[temp].phone, edit);
            }
        else{
            printf("Invalid phone number\n");
            }
            break;
        case 3:
         printf("Enter new email: ");
        scanf(" %99[^\n]", edit);

         if(email_validate(addressBook, edit)){
                strcpy(addressBook->contacts[temp].email, edit);
            }
        else{
            printf("Invalid email\n");
            }
            break;

        default:
            printf("Invalid choice\n");
    }
}

void deleteContact(AddressBook *addressBook){
    int temp;

    char confirm;

    printf("-------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\tENTERED INTO DELETE CONTACT\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    temp = searchAndSelectContact(addressBook);

    if(temp < 0){
        return;
    }
    displayContact(&addressBook->contacts[temp]);

    printf("\nDo you want to delete? (y/n): ");
    scanf(" %c", &confirm);
    while(getchar() != '\n');

    switch(confirm)
    {
        case 'y':
        case 'Y':
            for(int i = temp;i < addressBook->contactCount - 1;i++){
                addressBook->contacts[i] =
                addressBook->contacts[i + 1];
            }

            addressBook->contactCount--;
            printf("Contact deleted successfully\n");
            break;
        case 'n':
        case 'N':
        printf("Deletion cancelled\n");
            break;
        default:

            printf("Invalid input\n");
    }
}

void listContacts(AddressBook *addressBook){
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("\t\tENTERED INTO LIST CONTACT\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    printf("------------------------------------------------------------\n");
    printf("| %-15s | %-15s | %-30s |\n",
           "Name",
           "Phone",
           "Email");

    printf("------------------------------------------------------------\n");

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        printf("| %-15s | %-15s | %-30s |\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }

    printf("------------------------------------------------------------\n");
}

void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook);

    exit(EXIT_SUCCESS);
}