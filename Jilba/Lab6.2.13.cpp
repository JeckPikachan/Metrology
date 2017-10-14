// Lab6.2.13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctime>
#include <cstdio>
#include <iostream>
#include <conio.h>
#include <string>
#include <locale>
#include <windows.h>
#include <vector>

#define lint long long

using namespace std;

//////////////////////////////// S T R U C T U R E S //////////////////////////////////////

struct product {
	lint id;
	char name[80];
	lint clientId;
	double cost;
	double pledge;
	time_t delivered;
};

struct client {
	lint id;
	char name[80];
	lint passportSerialNum;
	time_t regestrationDate;
};


/////////////////////////////// F U N C T I O N S ////////////////////////////////////////////

void menu(vector<product> &products, vector<client> &clients);
void showProducts(vector<product> &products);
void showClients(vector<client> &clients);
void saveChanges(vector<product> &products, vector<client> &clients);
void loadInfo(vector<product> &products, vector<client> &clients);
void addProduct(vector<product> &products, int id = -1);
void deleteProduct(vector<product> &product, int id);
void addClient(vector<client> &clients, int id = -1);
void deleteClient(vector<client> &clients, int id);
void getRevenue(vector<product> &products);
void getTotalPriceOfProducts(vector<product> &products);

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	vector<product> productsVect;
	vector<client> clientsVect;
	int pSize, cSize;
	loadInfo(productsVect, clientsVect);

	menu(productsVect, clientsVect);
    return 0;
}

void menu(vector<product> &products, vector<client> &clients) {

	char choice;
	bool finished = false;

	while (!finished) {
		system("cls");
		cout << "\t\t\t����� ���������� � �������.���" << endl << endl;
		cout << "1. �������� �������� �������" << endl;
		cout << "2. �������� ������ ��������" << endl;
		cout << "3. ��������� ���������" << endl;
		cout << "4. ��������� ������� ��������" << endl;
		cout << "5. ��������� ��������� ���� ������� ��������" << endl;
		cout << "6. ����� �� ���������" << endl;

		choice = _getch();

		switch (choice) {
		case '1':
			showProducts(products);
			break;
		case '2':
			showClients(clients);
			break;
		case '3':
			saveChanges(products, clients);
			break;
		case '4':
			getRevenue(products);
			break;
		case '5':
			getTotalPriceOfProducts(products);
			break;
		case '6':
			saveChanges(products, clients);
			finished = true;
			break;
		}
	}
}

void loadInfo(vector<product> &products, vector<client> &clients) {
	FILE * pFile, * cFile;
	long pSize, cSize;

	pFile = fopen("products.bin", "rb");

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	pSize = ftell(pFile) / sizeof(product);
	rewind(pFile);

	// copy the file into the buffer:
	for (int i = 0; i < pSize; i++) {
		product* newProduct = new product;
		fread(newProduct, sizeof(product), 1, pFile);
		products.push_back(*newProduct);
	}
	
	fclose(pFile);

	cFile = fopen("clients.bin", "rb");

	fseek(cFile, 0, SEEK_END);
	cSize = ftell(cFile) / sizeof(client);
	rewind(cFile);

	for (int i = 0; i < cSize; i++) {
		client* newClient = new client;
		fread(newClient, sizeof(client), 1, cFile);
		clients.push_back(*newClient);
	}

	fclose(cFile);
}

void showProducts(vector<product> &products) {

	char choice;
	bool finished = false;

	while (!finished) {
		system("cls");
		cout << "\t1. ��������\t2. �������������\t3. �������\t4. ���������" << endl << endl;

		cout << "id\t�����\t\tid �������\t����\t�����\t���� �����" << endl;
		cout << "----------------------------------------------------------------------" << endl;

		for (int i = 0; i < products.size(); i++) {
			cout << products[i].id << "\t" << products[i].name << "\t" << products[i].clientId << "\t\t" << products[i].cost << "\t" << products[i].pledge << "\t" << ctime(&products[i].delivered);
		}

		choice = _getch();

		int id;

		switch (choice) {
		case '1':
			addProduct(products);
			break;
		case '2':
			system("cls");
			cout << "������� id ������, ������� ������ ��������: ";
			cin >> id;
			cin.ignore(65536, '\n');
			cin.clear();
			addProduct(products, id);
			break;
		case '3' :
			system("cls");
			cout << "������� id ������, ������� ������ �������: ";
			cin >> id;
			cin.ignore(65536, '\n');
			cin.clear();
			deleteProduct(products, id);
			break;
		case '4':
			finished = true;
			break;
		}
	}
}

void addProduct(vector<product> &products, int id) {
	if (id <= 0) {
		id = products.size();
		lint newId = !id ? 1 : products[id - 1].id + 1; 
		product newProduct{ newId, "", 0, 0, 0, time(NULL)};
		products.push_back(newProduct);
	}
	else {
		bool changed = false;
		for (int i = 0; i < products.size(); i++) {
			if (products[i].id == id) {
				id = i;
				changed = true;
				break;
			}
		}
		if (!changed) {
			cout << "�������� id";
			_getch();
			return;
		}
	}

	char buffer[80];

	cout << "������� �������� ������: "; gets_s(products[id].name, 79);
	cout << "������� id �������: "; cin >> products[id].clientId;
	cout << "������� ���� ������: "; cin >> products[id].cost;
	cout << "������� �����: "; cin >> products[id].pledge;

	cin.ignore(65536, '\n');
	cin.clear();

	cout << "\n����� ��������.";
	_getch();
}


void saveChanges(vector<product> &products, vector<client> &clients) {
	FILE *pFile, *cFile;

	pFile = fopen("products.bin", "wb");
	for (int i = 0; i < products.size(); i++) {
		fwrite(&products[i], sizeof(product), 1, pFile);
	}
	fclose(pFile);

	cFile = fopen("clients.bin", "wb");
	for (int i = 0; i < clients.size(); i++) {
		fwrite(&clients[i], sizeof(client), 1, cFile);
	}
	fclose(cFile);
}

void deleteProduct(vector<product> &products, int id) {
	bool changed = false;
	for (int i = 0; i < products.size(); i++) {
		if (products[i].id == id) {
			products.erase(products.begin() + i);
			changed = true;
			break;
		}
	}
	if (!changed) cout << "��� ������ ��������";
	else cout << "�������";
	_getch();
}

void showClients(vector<client> &clients) {
	char choice;
	bool finished = false;

	while (!finished) {
		system("cls");
		cout << "\t1. ��������\t2. �������������\t3. �������\t4. ���������" << endl << endl;

		cout << "id\t���\t\t\t����� ��������\t���� �����������" << endl;
		cout << "----------------------------------------------------------------------" << endl;

		for (int i = 0; i < clients.size(); i++) {
			cout << clients[i].id << "\t" << clients[i].name << "\t" << clients[i].passportSerialNum << "\t\t" << ctime(&clients[i].regestrationDate);
		}

		choice = _getch();

		int id;

		switch (choice) {
		case '1':
			addClient(clients);
			break;
		case '2':
			system("cls");
			cout << "������� id �������, �������� ������ ��������: ";
			cin >> id;
			cin.ignore(65536, '\n');
			cin.clear();
			addClient(clients, id);
			break;
		case '3':
			system("cls");
			cout << "������� id �������, �������� ������ �������: ";
			cin >> id;
			cin.ignore(65536, '\n');
			cin.clear();
			deleteClient(clients, id);
			break;
		case '4':
			finished = true;
			break;
		}
	}
}

void addClient(vector<client> &clients, int id) {
	if (id <= 0) {
		id = clients.size();
		lint newId = !id ? 1 : clients[id - 1].id + 1;
		client newClient{ newId, "", 0, time(NULL) };
		clients.push_back(newClient);
	}
	else {
		bool changed = false;
		for (int i = 0; i < clients.size(); i++) {
			if (clients[i].id == id) {
				id = i;
				changed = true;
				break;
			}
		}
		if (!changed) {
			cout << "�������� id";
			_getch();
			return;
		}
	}

	cout << "������� ��� �������: "; gets_s(clients[id].name, 79);
	cout << "������� ����� �������� �������: "; cin >> clients[id].passportSerialNum;

	cin.ignore(65536, '\n');
	cin.clear();

	cout << "\n������ ��������.";
	_getch();
}


void deleteClient(vector<client> &clients, int id) {
	bool changed = false;
	for (int i = 0; i < clients.size(); i++) {
		if (clients[i].id == id) {
			clients.erase(clients.begin() + i);
			changed = true;
			break;
		}
	}
	if (!changed) cout << "��� ������ ��������";
	else cout << "�������";
	_getch();
}

void getRevenue(vector<product> &products) {
	system("cls");

	int sum = 0;
	for (int i = 0; i < products.size(); i++) {
		sum += products[i].cost - products[i].pledge;
	}

	cout << "���������� ������� ��������: " << sum;
	_getch();
}

void getTotalPriceOfProducts(vector<product> &products) {
	system("cls");

	int sum = 0;
	for (int i = 0; i < products.size(); i++) {
		sum += products[i].cost;
	}

	cout << "��������� ��������� ���� ������� ��������: " << sum;
	_getch();
}