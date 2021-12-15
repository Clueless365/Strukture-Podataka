#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct MyStruct;
typedef struct MyStruct* pos;
typedef struct MyStruct
{
	int num;
	pos right;
	pos left;
} tree;

/*Koristen kod kao reference pa nije 100% original*/

pos createNew(int num);
pos insertEl(int num, pos root);
int printInOrder(pos root);
int printPreOrder(pos root);
int printPostOrder(pos root);
pos findMin(pos root);
pos findTree(pos root, int num);
pos deleteTree(pos root, int num);
int menu(pos root);

int main()
{
	pos root = NULL;
	menu(root);

	return EXIT_SUCCESS;
}

pos createNew(int num)
{
	pos newEl = (pos)malloc(sizeof(tree));
	if (newEl == NULL)
	{
		printf("Malloc greska\n");
		return NULL;
	}

	newEl->num = num;
	newEl->right = NULL;
	newEl->left = NULL;

	return newEl;
}

pos insertEl(int num, pos root)
{
	if (!root)
		return createNew(num);

	else if (num < root->num)
		root->left = insertEl(num, root->left);

	else if (num > root->num)
		root->right = insertEl(num, root->right);

	return root;
}

int printInOrder(pos root)
{
	if (root != NULL)
	{
		printInOrder(root->left);
		printf("%d\n", root->num);
		printInOrder(root->right);
	}
	return EXIT_SUCCESS;
}

int printPreOrder(pos root)
{
	if (root != NULL)
	{
		printf("%d\n", root->num);
		printPreOrder(root->left);
		printPreOrder(root->right);
	}
	return EXIT_SUCCESS;
}

int printPostOrder(pos root)
{
	if (root != NULL)
	{
		printPostOrder(root->left);
		printPostOrder(root->right);
		printf("%d\n", root->num);
	}
	return EXIT_SUCCESS;
}

pos deleteTree(pos root, int num)
{
	pos temp;

	if (root == NULL)
		return NULL;

	else if (num < root->num)
		root->left = deleteTree(root->left, num);

	else if (num > root->num)
		root->right = deleteTree(root->right, num);

	else
	{
		if (root->left && root->right)
		{
			temp = findMin(root->right);
			root->num = temp->num;
			root->right = deleteTree(root->right, temp->num);
		}
		else
		{
			temp = root;
			if (root->left == NULL )
				root = root->right;

			else if (root->right == NULL )
				root = root->left;

			free(temp);
		}
	}

	return root;
}

pos findMin(pos root)
{
	while (root->left)
		root = root->left;

	return root;
}

pos findTree(pos root, int num)
{
	if (root == NULL)
		return NULL;

	else if (num < root->num)
		return findTree(root->left, num);

	else if (num > root->num)
		return findTree(root->right, num);

	else
		return root;
}
int menu(pos root)
{
	int choice = 0;
	int num = 0;
	int check = 0;
	pos temp = NULL;

	while (1)
	{
		printf(">IZBOR:\n"
			"1 - Dodaj novi element\n"
			"2 - Inorder print\n"
			"3 - Preorder print\n"
			"4 - postorder print\n"
			"5 - Pronadi element\n"
			"6 - Izbrisi element\n"
			"0 - exit\n");
		printf("> ");
		scanf(" %d", &choice);

		switch (choice)
		{
		case 1:
			printf("Unesi br: ");
			check = scanf(" %d", &num);
			if (check != 1)
			{
				printf("Greska \nexiting...");
				return EXIT_FAILURE;
			}

			root = insertEl(num, root);
			break;

		case 2:
			printInOrder(root);
			break;

		case 3:
			printPreOrder(root);
			break;

		case 4:
			printPostOrder(root);
			break;

		case 5:
			printf("Pronadi br: ");
			check = scanf(" %d", &num);
			if (check != 1)
			{
				printf("Greska \nexiting...");
				return EXIT_FAILURE;
			}


			temp = findTree(root, num);

			if (temp != NULL)
				printf("Naden br %d na mjestu %p\n", temp->num, temp);
			else
				printf("Nije pronaden\n");

			break;

		case 6:
			printf("Izbrisi br: ");
			check = scanf(" %d", &num);
			if (check != 1)
			{
				printf("Greska \nexiting...");
				return EXIT_FAILURE;
			}


			root = deleteTree(root, num);
			break;

		case 0:
			printf("exiting...\n\n");
			return EXIT_SUCCESS;

		default:
			menu(root);
		}
	}

	return EXIT_SUCCESS;
}