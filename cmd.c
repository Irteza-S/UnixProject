#include "cmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Prints the command
void printCmd(cmd *cmd)
{
	printf(" \n init_cmd = %s \n ",cmd->initCmd);
	printf(" \n nb_cmd_members = %d \n ",cmd->nbCmdMembers);
	for(int i=0; i<cmd->nbCmdMembers; i=i+1)
	{
		printf(" \n cmd_members[%d] = %s \n ",i,cmd->cmdMembers[i]);
	}
}

//Initializes the initial_cmd, membres_cmd et nb_membres fields
void parseMembers(char *inputString,cmd *cmd)
{

	    // Fill inputstring
  	    cmd->initCmd = strdup(inputString);

	   // Check the number of members
       unsigned int nbMembers = 1;
       int m =0;
        while (inputString[m] != '\0')
        {
        if (inputString[m] == '|')
        {
            ++nbMembers;
        }
        ++m;
        }
        cmd->nbCmdMembers = nbMembers;


	    // Fill the array "cmdMembers"
        cmd->cmdMembers = (char **)malloc (nbMembers * sizeof (char *));


        char *token;
        int index =0;
      	token = strtok(inputString, "|");
        cmd->cmdMembers[index] = token;
        index = index +1;

        while( index < nbMembers)
        {
          token = strtok(NULL, "|");
          cmd->cmdMembers[index] = token;
          index = index +1;

        }
    printCmd(cmd);

		// Parsing command
	/*	cmd->cmdMembers = (char **)malloc (nbMembers * sizeof (char *));
		for(int i =0; i<nbMembers; i=i+1)
		{
			cmd->cmdMembers[i] = malloc(5* sizeof(char));
		}*/
		//tab[i] = malloc(* sizeof(char));

		printf("\n on entre dans la boucle ");
		char *tokenCmdMembersArg;
    int indexCmdMembersArg = 0;
    for (int i = 0 ; i < nbMembers ; ++i)
    {

        	int stop = 0;
        	printf("\n  Chaine a pare :  %s", cmd->cmdMembers[i]);
	    	char *tmp = cmd->cmdMembers[i];
	    	tokenCmdMembersArg = strtok(tmp, " ");
		length = strlen (tokenCmdMembersArg);
        	cmd->cmdMembersArgs[loopIndex][tokenCmdMembersArg] = (char *) malloc (length * sizeof (char));
	    while (tokenCmdMembersArg != NULL && stop == 0)
	    {

	                ++indexCmdMembersArg;

			char *pch = strpbrk(tokenCmdMembersArg, "<>");
			if (pch != NULL)
			{

                cmd->cmdMembersArgs[i][indexCmdMembersArg] = NULL;
                stop = 1;
			}
			else
			{
                cmd->cmdMembersArgs[i][indexCmdMembersArg] = tokenCmdMembersArg;
			}
			tokenCmdMembersArg = strtok (NULL, " ");
        }


    }
}

//Frees memory associated to a cmd
void freeCmd(cmd  * cmd){

}
