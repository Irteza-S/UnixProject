#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Prints the command
void printCmd(cmd *cmd)
{
    int i, j;
    int fin = cmd->nbCmdMembers;
    printf ("\nCommande entrée : %s\n", cmd->initCmd);
    printf ("\nNombre de membre(s) : %d\n", cmd->nbCmdMembers);
    for (i = 0 ; i < fin ; ++i)
    {
        printf ("\nSous-commande %d : %s\n", i, cmd->cmdMembers[i]);
    }
/*
    for (i = 0 ; i < fin ; ++i)
    {
        for (j = 0 ; cmd->cmdMembersArgs[i][j] == NULL; ++j)
        {
            printf ("\nArguments %d de la sous-commande %d : %s\n", j, i, cmd->cmdMembersArgs[i][j]);
        }
    }
    for (i = 0 ; i < fin ; ++i)
    {
        printf ("\nNombre d'argument de la sous-commande %d : %d\n", i, cmd->nbMembersArgs[i]);
    }
    for (i = 0 ; i < fin ; ++i)
    {
        for (j = 0 ; j == 2 ; ++j)
        {
        printf ("\nRedirection %d - %d : %s\n", i, j, cmd->redirection[i][j]);
        }
    }
*/
}
//Initializes the initial_cmd, membres_cmd et nb_membres fields
void parseMembers(char *inputString,cmd *cmd)
{
    // Pour les boucles "for"
    int loopIndex;
    // Recopier la commande saisie dans la structure
    cmd->initCmd = strdup(inputString);
    // Récupérer le nombre de membres
    int nbMembers = 1;
    int posIndex = 0;
    while (inputString[posIndex] != '\0')
    {
        if (inputString[posIndex] == '|') // nbMembers = nombre de "|" + 1
        {
            ++nbMembers;
        }
        ++posIndex;
    }
    cmd->nbCmdMembers = nbMembers;
    // Remplir le tableau des membres "cmdMembers"
    char *tokenCmdMembers;
    int indexCmdMembers = 0;
    cmd->cmdMembers = (char **) malloc (nbMembers * sizeof (char *));
    tokenCmdMembers = strtok(inputString, "|"); // on ne tient pas compte (pour le moment) des espaces
    cmd->cmdMembers[indexCmdMembers] = tokenCmdMembers;
    while (indexCmdMembers < nbMembers)
    {
        ++indexCmdMembers;
        tokenCmdMembers = strtok(NULL, "|"); 
        cmd->cmdMembers[indexCmdMembers] = tokenCmdMembers;
    }
    // Remplir le tableau des arguments de chaque sous-commande "cmdMembersArgs"
    char *tokenCmdMembersArgs;
    int indexCmdMembersArgs = 0;
    for (loopIndex = 0 ; loopIndex < nbMembers ; ++loopIndex)
    {
        char *tmp = cmd->cmdMembers[loopIndex];
        int stop = 0;
        tokenCmdMembersArgs = strtok(tmp, " ");
        cmd->cmdMembersArgs[loopIndex][indexCmdMembersArgs] = strdup(tokenCmdMembersArgs);
        tokenCmdMembersArgs = strtok (NULL, " ");
        while (tokenCmdMembersArgs != NULL && stop == 0)
        {
            ++indexCmdMembersArgs;
        
            char *pch = strpbrk(tokenCmdMembersArgs, "<>");
            if (pch != NULL)
            {
                cmd->cmdMembersArgs[loopIndex][indexCmdMembersArgs] = NULL;
                stop = 1;
            }
            else
            {
                cmd->cmdMembersArgs[loopIndex][indexCmdMembersArgs] = strdup(tokenCmdMembersArgs);
                tokenCmdMembersArgs = strtok (NULL, " ");
            }
        }
    }
    // Récupérer le nombre d'arguments par sous-commande "nbMembersArgs"
    int nbMembersArgs = 1;
    int indexMember, indexArg = 0;
    for (indexMember = 0 ; indexMember == nbMembers ; ++indexMember)
    {
        while (cmd->cmdMembersArgs[indexMember][indexArg] != NULL)
        {
            ++indexArg;
            ++nbMembersArgs;
        }
        cmd->nbMembersArgs[indexMember] = nbMembersArgs;
        nbMembersArgs = 1;
        indexArg = 0;
    }
    // Remplir le tableau des redirections "redirection"
    cmd->redirection = (char ***) malloc (nbMembers * sizeof (char **));
    
    char *tokenCmdMembersRedirection;
    for (loopIndex = 0 ; loopIndex < nbMembers ; ++loopIndex)
    {
        char *tmp = cmd->cmdMembers[loopIndex];
        tokenCmdMembersRedirection = strtok(tmp, " ");
        tokenCmdMembersRedirection = strtok (NULL, " ");
        if (tokenCmdMembersRedirection[0] == '<')
        {
            tokenCmdMembersRedirection = strtok (NULL, " ");
            cmd->redirection[loopIndex][0] = strdup(tokenCmdMembersRedirection);
            tokenCmdMembersRedirection = strtok (NULL, " ");
            cmd->redirection[loopIndex][1] = strdup(tokenCmdMembersRedirection);
            cmd->redirection[loopIndex][2] = strdup(tokenCmdMembersRedirection);
        }
        if (tokenCmdMembersRedirection[0] == '>')
        {
            tokenCmdMembersRedirection = strtok (NULL, " ");
            cmd->redirection[loopIndex][1] = strdup(tokenCmdMembersRedirection);
            tokenCmdMembersRedirection = strtok (NULL, " ");
            cmd->redirection[loopIndex][0] = strdup(tokenCmdMembersRedirection);
            cmd->redirection[loopIndex][2] = strdup(tokenCmdMembersRedirection);
        }
        if (tokenCmdMembersRedirection[0] == '2')
        {
            tokenCmdMembersRedirection = strtok (NULL, " ");
            cmd->redirection[loopIndex][2] = tokenCmdMembersRedirection;
            tokenCmdMembersRedirection = strtok (NULL, " ");
            cmd->redirection[loopIndex][0] = strdup(tokenCmdMembersRedirection);
            cmd->redirection[loopIndex][1] = strdup(tokenCmdMembersRedirection);
        }
    }
    // Récupérer le type de redirection "redirectionType"
    for (loopIndex = 0 ; loopIndex < nbMembers ; ++loopIndex)
    {
        char *tmp = cmd->cmdMembers[loopIndex];
        tokenCmdMembersRedirection = strtok(tmp, " ");
        tokenCmdMembersRedirection = strtok (NULL, " ");
        if (tokenCmdMembersRedirection[0] == '>')
        {
            if (tokenCmdMembersRedirection[1] == '>')
            {
                cmd->redirectionType[loopIndex][1] = APPEND;
            }
            else
            {
            cmd->redirectionType[loopIndex][1] = OVERRIDE;
            }
        }
    }
}
//Frees memory associated to a cmd
void freeCmd(cmd  * cmd){
  
}

