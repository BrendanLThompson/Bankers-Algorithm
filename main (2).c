#include <stdio.h>
#include <stdlib.h>

int numResources;
int numProcesses;
int *resource = NULL;
int *available = NULL;
int *allocated = NULL;
int *maxClaim = NULL;
int *need = NULL;

void resourcePrint() {
  int i;
  printf("Resources: \n");
  for(i=0; i < numResources; i++){
    printf("\tr%d",i);
  }
  printf("\n");
  for(i=0; i < numResources; i++){
    printf("\t%d", resource[i]);
  }
  printf("\n");
	return;
}

void availablePrint() {
  int i;
	printf("Available: \n");
  for(i=0; i < numResources; i++){
    printf("\tr%d",i);
  }
  printf("\n");
  for(i=0; i < numResources; i++){
    printf("\t%d", available[i]);
  }
  printf("\n");
	return;
}

void maxPrint() { 
	int i,j;
	printf("Max claim: \n");
	for (i = 0; i < numResources; i++) {
		printf("\tr%d", i);
	}
	printf("\t");
	printf("\n");
	for (i = 0; i < numProcesses; i++) {
		printf("p%d\t", i);
		for (j = 0; j < numResources; j++ ) {
			printf("%d\t", maxClaim[i * numResources + j]);
		}
		printf("\n");
	}
	printf("\n");		
	return;
}

void allocationPrint() {
  int i,j;
	printf("Allocated: \n");
	for (i = 0; i < numResources; i++) {
		printf("\tr%d", i);
	}
	printf("\t");
	printf("\n");
	for (i = 0; i < numProcesses; i++) {
		printf("p%d\t", i);
		for (j = 0; j < numResources; j++ ) {
			printf("%d\t", allocated[i * numResources + j]);
		}
		printf("\n");
	}
	printf("\n");	
    return;
}

void needPrint() {
	int i,j;
	printf("Need: \n");
	for (i = 0; i < numResources; i++) {
		printf("\tr%d", i);
	}
	printf("\t");
	printf("\n");
	for (i = 0; i < numProcesses; i++) {
		printf("p%d\t", i);
		for (j = 0; j < numResources; j++ ) {
			printf("%d\t", need[i * numResources + j]);
		}
		printf("\n");
	}
	printf("\n");	
}

void option1() {
	/* declare local variables */
    int i, j, units;
	/* prompt for number of processes and number of resources */
  printf("Enter Number of Processes: ");
  scanf("%d", &numProcesses);
  printf("Enter Number of Resources: ");
  scanf("%d", &numResources);
	/* allocate memory for arrays and arrays: resource, available, max_claim, allocated, need */
  resource = (int *)malloc(numResources * sizeof(int));
  available = (int *)malloc(numResources * sizeof(int));
  maxClaim = (int *)malloc(numProcesses * numResources * sizeof(int));
  allocated = (int *)malloc(numProcesses * numResources * sizeof(int));
  need = (int *)malloc(numProcesses * numResources * sizeof(int));
	/* for each resource, prompt for number of units, set resource and available arrays indices*/
  for(i = 0; i < numResources; i++){
    printf("Enter number of units for resource r%d: ", i);
    scanf("%d", &units);
    printf("%d\n", units);
    resource[i] = units;
    available[i] = units;
  }
	/* for each process, for each resource, prompt for maximum number of units requested by process, update max_claim and need arrays */ 
  for(i = 0; i < numProcesses; i++){
    for(j = 0; j < numResources; j++){
      printf("Enter maximum number of units process p%d will request from resource r%d: ", i, j);
      scanf("%d", &units);
      printf("%d\n", units);
      maxClaim[i * numResources + j] = units;
      need[i * numResources + j] = units;
    }
  }
	/* for each process, for each resource, prompt for number of resource units allocated to process */
  for(i = 0; i <numProcesses; i++){
    for(j = 0; j < numResources; j++){
      printf("Enter number of resource r%d allocated to process p%d: ", j, i);
      scanf("%d", &units);
      printf("%d\n", units);
      allocated[i * numResources + j] = units;
      need[i * numResources + j] -= units;
      available[j] -= units;
    }
  }
	/* call procedures to print resource array, available array, max_claim array, allocated array, need array */
  resourcePrint();
  availablePrint();
  maxPrint();
  allocationPrint();
  needPrint();
	return;
}


/********************************************************************/
void option2() {
 
	/* declare local variables, including array to indicate if process is safely sequenced and "done" counter*/
	int done = 0;
	int lessThan;
	int i,j;
	int *sequenced = (int *)calloc(numProcesses, sizeof(int));
	/* while not done */
	while (done < numProcesses){
		/* for each process to check for safe sequencing */
		for (i = 0; i < numProcesses; i++){
			lessThan = 1;
			/*. not already put in a safe sequence */
			if (sequenced[i] == 0){
				printf("Checkin p%d: <", i);
				/* for each resource, print need array entry */
				for (j = 0; j < numResources; j++){
					printf(" %d ", need[i * numResources + j]);
				}
				printf(" > <= < ");
				/* for each resource, print available array entry */
				for (j = 0; j < numResources; j++){
					printf(" %d ", available[j]);
				}
				printf(" > ");
				/* for each resource, check if need array entry is less than available array entry */
				for( j = 0; j< numResources; j++){
					lessThan = lessThan & (need[i * numResources + j] <= available[j]);
				}
				/* if all need entries are less than the corresponding available entry */ 
				if (lessThan == 1){ 
					/* put process in safe sequence for each resource */
					printf(" :p%d safely sequenced\n", i);
					sequenced[i] = 1;

					for (j=0; j < numResources; j++){
						/* adjust units in available array */
						available[j] += allocated[i * numResources + j];
						/* reset units in allocated array */
						allocated[i * numResources + j] = 0;
						need[i * numResources + j] = maxClaim[i * numResources + j];
						
						/* increment "done" counter */
						done++;
					}
				}
				printf("\n");		
			}			
		}				
	}	
	printf("\n");
	free(sequenced);
	return;
}


/********************************************************************/
int main()
{
	int choice = 0;
	do
	{
		printf("\n\nBankers Algorithm\n"
			   "1) Enter Parameters\n"
			   "2) Determine Safe Sequence\n"
			   "3) Exit\n\n"
			   "Enter Selection:\n");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			option1();
			break;
		case 2:
			option2();
			break;
		case 3:
			printf("\nQuiting\n");
      break;
		default:
			printf("Invalid option\n");
		}
	} while (choice != 3);
	return 1;
}
