#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<algorithm>
#include<conio.h>
#include<queue>
using namespace std;
#define pi 3.14
struct City {
	string name;
	double lat = 0; // latitude
	double lng = 0; // longitude
};
// Convert string to double
double stod_(string s)
{
	double res = 0;
	stringstream ss(s);
	bool isNegative = false;
	if (s[0] == '-')
	{
		ss.ignore(1);
		isNegative = true;
	}
	ss >> res;
	return isNegative ? -res : res;
}
struct Node
{
	Node* left = NULL;
	Node* right = NULL;
	City key;
};
Node* createNode(City city)
{
	Node* newNode = new Node;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->key = city;
	return newNode;
}
//Insert thanh pho vao cay
void Insert(Node*& root, City city, bool& isDuplicate, int depth = 0)
{
	if (root == NULL)
	{
		Node* newNode = createNode(city);
		root = newNode;
		return;
	}
	int k = depth % 2;
	//Neu cay co thu tu chan
	if (k == 0)
	{
		if (city.lat < root->key.lat)
			Insert(root->left, city, isDuplicate, depth + 1);
		else if (city.lat > root->key.lat)
			Insert(root->right, city, isDuplicate, depth + 1);
		else if (city.lat == root->key.lat && city.lng != root->key.lng)
			Insert(root->left, city, isDuplicate, depth + 1);
		else
		{
			isDuplicate = true;
			return;
		}
	}
	//Neu cay co thu tu le
	else
	{
		if (city.lng < root->key.lng)
			Insert(root->left, city, isDuplicate, depth + 1);
		else if (city.lng > root->key.lng)
			Insert(root->right, city, isDuplicate, depth + 1);
		else if (city.lng == root->key.lng && city.lat != root->key.lat)
			Insert(root->left, city, isDuplicate, depth + 1);
		else
		{
			isDuplicate = true;
			return;
		}
	}
}
//Tinh khoang cach theo cong thuc haversine ma bien truyen vao la city voi toa do
double distance(City city, double pos[2])
{
	// Change degree to radian
	double lat1 = city.lat * pi / 180.0;
	double lng1 = city.lng * pi / 180.0;
	double lat2 = pos[0] * pi / 180.0;
	double lng2 = pos[1] * pi / 180.0;

	double dLat = lat2 - lat1;
	double dLng = lng2 - lng1;

	double a = pow(sin(dLat / 2), 2) + pow(sin(dLng / 2), 2) * cos(lat1) * cos(lat2);

	double c = 2 * asin(sqrt(a));
	double R = 6371.0;

	return R * c;
}
//Tinh khoang cach theo cong thuc haversine ma bien truyen vao la 2 city
double distance(City city1, City city2) {

	// Change degree to radian
	double lat1 = city1.lat * pi / 180.0;
	double lng1 = city1.lng * pi / 180.0;
	double lat2 = city2.lat * pi / 180.0;
	double lng2 = city2.lng * pi / 180.0;

	double dLat = lat2 - lat1;
	double dLng = lng2 - lng1;

	double a = pow(sin(dLat / 2), 2) + pow(sin(dLng / 2), 2) * cos(lat1) * cos(lat2);

	double c = 2 * asin(sqrt(a));

	double R = 6371.0;

	return R * c;
}
//Tinh khoang cach theo cong thuc haversine ma bien truyen vao la 2 toa do
double distance(double pos1[2], double pos2[2]) {

	// Change degree to radian
	double lat1 = pos1[0] * pi / 180.0;
	double lng1 = pos1[1] * pi / 180.0;
	double lat2 = pos2[0] * pi / 180.0;
	double lng2 = pos2[1] * pi / 180.0;

	double dLat = lat2 - lat1;
	double dLng = lng2 - lng1;

	double a = pow(sin(dLat / 2), 2) + pow(sin(dLng / 2), 2) * cos(lat1) * cos(lat2);

	double c = 2 * asin(sqrt(a));

	double R = 6371.0;

	return R * c;
}
//Check xem thanh pho do co thuoc mien hinh chu nhat khong
bool isInRectangle(City city, double posRectangel1[2], double posRectangel2[2])
{
	if (city.lat < min(posRectangel1[0], posRectangel2[0]) || city.lat > max(posRectangel1[0], posRectangel2[0]))
		return false;
	if (city.lng < min(posRectangel1[1], posRectangel2[1]) || city.lng > max(posRectangel1[1], posRectangel2[1]))
		return false;
	return true;
}
//Check cay root co thanh pho nao thuoc mien chu nhat neu co thi luu vao vector city
void RangeSearch(Node* root, vector<City>& city, double posRectangel1[2], double posRectangel2[2], int depth = 0)
{
	if (root == NULL)
		return;
	int k = depth % 2;
	//Neu node do o thu tu chan thi so lat
	if (k == 0)
	{
		//Neu lat nho hon min thi chac chan khong trong hinh chu nhat
		if (root->key.lat < min(posRectangel1[k], posRectangel2[k]))
		{
			RangeSearch(root->right, city, posRectangel1, posRectangel2, depth + 1);
		}
		//Neu lat lon hon max thi chac chan khong trong hinh chu nhat
		else if (root->key.lat > max(posRectangel1[k], posRectangel2[k]))
		{
			RangeSearch(root->left, city, posRectangel1, posRectangel2, depth + 1);
		}
		//Neu trong hinh chu nhat thi bo vao vector
		else if (isInRectangle(root->key, posRectangel1, posRectangel2))
		{
			city.push_back(root->key);
			RangeSearch(root->left, city, posRectangel1, posRectangel2, depth + 1);
			RangeSearch(root->right, city, posRectangel1, posRectangel2, depth + 1);
		}
		else
		{
			RangeSearch(root->left, city, posRectangel1, posRectangel2, depth + 1);
			RangeSearch(root->right, city, posRectangel1, posRectangel2, depth + 1);
		}
	}
	//Neu node do o thu tu le thi so y
	else if (k == 1)
	{
		if (root->key.lng < min(posRectangel1[k], posRectangel2[k]))
		{
			RangeSearch(root->right, city, posRectangel1, posRectangel2, depth + 1);
		}
		else if (root->key.lng > max(posRectangel1[k], posRectangel2[k]))
		{
			RangeSearch(root->left, city, posRectangel1, posRectangel2, depth + 1);
		}
		else if (isInRectangle(root->key, posRectangel1, posRectangel2))
		{
			city.push_back(root->key);
			RangeSearch(root->left, city, posRectangel1, posRectangel2, depth + 1);
			RangeSearch(root->right, city, posRectangel1, posRectangel2, depth + 1);
		}
		else
		{
			RangeSearch(root->left, city, posRectangel1, posRectangel2, depth + 1);
			RangeSearch(root->right, city, posRectangel1, posRectangel2, depth + 1);
		}
	}
}
//Tim kiem lang gieng gan voi toa do nhat
void NearestNeighborSearch(Node* root, City& city, double pos[2], double& bestDistance, int depth = 0)
{
	if (root == NULL)
		return;
	int k = depth % 2;
	double d = distance(root->key, pos);
	//Neu distance be hon best Distance thi luu lai trang thai
	if (d < bestDistance)
	{
		bestDistance = d;
		city = root->key;
	}
	if (k == 0)
	{
		//Neu lat node lon hon thi di ben trai check
		if (pos[k] < root->key.lat)
		{
			NearestNeighborSearch(root->left, city, pos, bestDistance, depth + 1);
			double diff = 2 * 6371.0 * asin(sin((fabs((pos[k] - root->key.lat)) * pi / 180) / 2));
			//Neu nhu chi so voi lat ma diff da lon hon roi thi chac chan co lng thi no se lon hon. Neu nhu so voi lat ma diff nho hon di nhanh do
			if (diff < bestDistance) {
				NearestNeighborSearch(root->right, city, pos, bestDistance, depth + 1);
			}
		}
		else {
			NearestNeighborSearch(root->right, city, pos, bestDistance, depth + 1);
			double diff = 2 * 6371.0 * asin(sin((fabs((pos[k] - root->key.lat)) * pi / 180) / 2));
			if (diff < bestDistance) {
				NearestNeighborSearch(root->left, city, pos, bestDistance, depth + 1);
			}
		}
	}
	else
	{
		if (pos[k] < root->key.lng)
		{
			NearestNeighborSearch(root->left, city, pos, bestDistance, depth + 1);
			double diff = 2 * 6371.0 * asin(sin((fabs((pos[k] - root->key.lng)) * pi / 180) / 2));
			if (diff < bestDistance) {
				NearestNeighborSearch(root->right, city, pos, bestDistance, depth + 1);
			}
		}
		else {
			NearestNeighborSearch(root->right, city, pos, bestDistance, depth + 1);
			double diff = 2 * 6371.0 * asin(sin((fabs((pos[k] - root->key.lng)) * pi / 180) / 2));
			if (diff < bestDistance) {
				NearestNeighborSearch(root->left, city, pos, bestDistance, depth + 1);
			}
		}
	}
}
bool compareLat(City a, City b) {
	return a.lat < b.lat;
}
bool compareLng(City a, City b)
{
	return a.lng < b.lng;
}
void buildNLR(vector<City>v, vector<City>& curr, int depth = 0)
{
	if (v.empty())
		return;
	int k = depth % 2;
	//Neu nhu bac chan thi sort vector tu nho hon den lon hon so voi lat
	if (k == 0)
		sort(v.begin(), v.end(), compareLat);
	else
		sort(v.begin(), v.end(), compareLng);
	int median = v.size() / 2;
	//Tao node voi median
	curr.push_back(v[median]);

	vector<City> leftCity(v.begin(), v.begin() + median);
	vector<City> rightCity(v.begin() + median + 1, v.end());

	buildNLR(leftCity, curr, depth + 1);
	buildNLR(rightCity, curr, depth + 1);
}
void buildKDTree(Node*& root, vector<City>v)
{
	vector<City>curr;
	buildNLR(v, curr);
	bool isDuplicate = false;
	for (int i = 0; i < curr.size(); i++)
	{
		Insert(root, curr[i], isDuplicate);
	}
}
// ================= INTERFACE ======================/ 
void clearBinaryFile(string filePath) {
	ofstream file(filePath, std::ios::binary | std::ios::out | std::ios::trunc);
	if (file.is_open()) {
		file.close();
	}
}
bool isUTF8(string s)
{
	for (int x : s)
	{
		if (x < 0 || x > 255) return true;
	}
	return false;
}

City readCity(string read)
{
	City tmp_city;
	string tmp;
	stringstream ss(read);

	getline(ss, tmp_city.name, ','); // rx`ead city's name

	getline(ss, tmp, ','); // read latitude
	tmp_city.lat = stod_(tmp);

	getline(ss, tmp, ','); // read longitude
	tmp_city.lng = stod_(tmp);
	return tmp_city;
}
//Doc file roi luu vao vector
vector<City> readFile(string filename) {
	vector<City> data;
	string read;
	City tmp_city;
	ifstream f(filename.c_str());

	if (!f.is_open())
	{
		cout << "Can not open file\n";
		return data;
	}

	getline(f, read);

	while (getline(f, read)) {
		// If read is UTF8 , continue; 
		if (isUTF8(read)) continue;
		tmp_city = readCity(read);
		data.push_back(tmp_city);
	}
	f.close();
	return data;
}
//Doc file roi luu vao vector
vector<City> readFile1(string filename) {
	vector<City> data;
	string read;
	City tmp_city;
	ifstream f(filename.c_str());

	if (!f.is_open())
	{
		cout << "Can not open file\n";
		return data;
	}


	while (getline(f, read)) {
		// If read is UTF8 , continue; 
		if (isUTF8(read)) continue;
		tmp_city = readCity(read);
		data.push_back(tmp_city);
	}
	f.close();
	return data;
}
void inputCity(Node*& root)
{
	if (root == NULL)
	{
		cout << "Please, load a list of cities from a CSV file\n";
		return;
	}
	City data;
	bool isDuplicate = false;
	cin.ignore();
	cout << "Input city: "; getline(cin, data.name);
	cout << "Input latitude: "; cin >> data.lat;
	cout << "Input longitude: "; cin >> data.lng;
	Insert(root, data, isDuplicate);
	// check trùng toạ độ chưa check trùng tên
	if (isDuplicate == true)
	{
		cout << "City is exist\n";
		return;
	}
	cout << "Insert a new city is successful";
}
// If duplicate is skip, only insert city that no duplicate
void insertMultipleCities(string fileName, Node*& root)
{
	vector<City> temp = readFile1(fileName);
	for (int i = 0; i < temp.size(); i++)
	{
		bool isDuplicate = false;
		Insert(root, temp[i], isDuplicate);

	}
	return;
}
void printNearestNeighborSearch(Node* root, string file)
{
	if (root == NULL)
	{
		cout << "Please, load a list of cities from a CSV file\n";
		return;
	}
	ofstream out(file);
	double pos[2];
	cout << "Input latitude: "; cin >> pos[0];
	cout << "Input longitude: "; cin >> pos[1];
	City data;
	double bestDistance = INT32_MAX;
	NearestNeighborSearch(root, data, pos, bestDistance);
	cout << "Nearest neighbor search: \n";
	cout << data.name << "," << data.lat << "," << data.lng << endl;
	out << data.name << "," << data.lat << "," << data.lng << endl;
}
void printRangeSearch(Node* root, string fileName)
{
	if (root == NULL)
	{
		cout << "Please, load a list of cities from a CSV file\n";
		return;
	}
	ofstream out(fileName);
	double posRectangel1[2];
	double posRectangel2[2];
	cout << "Input bottom-left corners (latitude longitude): ";
	cin >> posRectangel1[0] >> posRectangel1[1];
	cout << "Input top-right corners (latitude longitude): ";
	cin >> posRectangel2[0] >> posRectangel2[1];
	vector<City> rectangel;
	RangeSearch(root, rectangel, posRectangel1, posRectangel2);
	if (rectangel.size() == 0)
	{
		cout << "No city is within this region\n";
	}
	else
	{
		cout << "List all cities within a specified rectangular region: \n";
		for (int i = 0; i < rectangel.size(); i++)
		{
			out << rectangel[i].name << "," << rectangel[i].lat << "," << rectangel[i].lng << endl;
			cout << rectangel[i].name << "," << rectangel[i].lat << "," << rectangel[i].lng << endl;
		}
	}
	out.close();
}
void printLevelOrder(Node* root)
{
	if (root == NULL)
	{
		cout << "Please, load a list of cities from a CSV file\n";
		return;
	}
	queue<Node*> q;
	q.push(root);
	int level = 0;
	while (!q.empty())
	{
		int n = q.size();
		cout << "Level " << level << ": \n";
		while (n--)
		{
			Node* first = q.front();
			cout << first->key.name << " " << first->key.lat << " " << first->key.lng << endl;
			q.pop();
			if (first->left)
			{
				q.push(first->left);
			}
			if (first->right)
			{
				q.push(first->right);
			}
		}
		level++;
	}
}

void deleteAllNodes(Node*& root)
{
	if (root == NULL)
	{
		return;
	}
	deleteAllNodes(root->left);
	deleteAllNodes(root->right);
	if (root->left == NULL && root->right == NULL)
	{
		delete root;
		root = NULL;
	}
}
// ============ EXTENSIONS =================

// Save vector following level order
vector<City> levelOrder(Node* root)
{
	vector<City> v;
	if (root == NULL) return v;
	queue<Node*> q;
	q.push(root);
	while (!q.empty())
	{
		Node* first = q.front();
		v.push_back(first->key);
		q.pop();
		if (first->left)
		{
			q.push(first->left);
		}
		if (first->right)
		{
			q.push(first->right);
		}
	}
	return v;
}
// Save KD tree into binary file
void saveKDTree(Node* root, string fileName)
{
	ofstream out(fileName, ios::binary | ios::trunc);
	if (root == NULL)
	{
		cout << "Please, load a list of cities from a CSV file\n";
		return;
	}
	vector<City> data = levelOrder(root);
	for (int i = 0; i < data.size(); i++)
	{
		int n = data[i].name.size();
		out.write((char*)&n, sizeof(n));
		out.write(data[i].name.c_str(), n);
		out.write((char*)&data[i].lat, sizeof(data[i].lat));
		out.write((char*)&data[i].lng, sizeof(data[i].lng));
	}
	cout << "Save the KDTree to a binary file is successful\n";
	out.close();
}
// Read KD Tree from binary file
Node* readKDTree(string filename)
{
	ifstream f(filename, ios::binary);
	if (!f.good())
	{
		cout << "File open error!\n";
		return nullptr;
	}

	Node* root = nullptr;
	bool isDuplicate = false;
	City temp;
	int size;

	while (f.good())
	{
		size = 0;
		f.read((char*)&size, sizeof(size));
		if (!size) break;

		temp.name.resize(size);
		f.read(&temp.name[0], size);

		f.read((char*)&temp.lat, sizeof(double));
		f.read((char*)&temp.lng, sizeof(double));
		Insert(root, temp, isDuplicate);
	}
	if (root == NULL)
	{
		cout << "There is no information in the binary file\n";
	}
	f.close();
	return root;
}
void interface()
{
	vector<City> city;
	Node* root = NULL;
	while (true)
	{
		system("cls");
		cout << "00. Exit\n";
		cout << "01. Load a list of cities from a CSV file \n";
		cout << "02. Insert a new city into the KD-Tree directly via the command line\n";
		cout << "03. Insert multiple new cities into the KD-Tree from a specified CSV file path\n";
		cout << "04. Conduct a nearest neighbor search by providing latitude and longitude coordinates\n";
		cout << "05. Query all cities within a specified rectangular region defined by its geographical boundaries\n";
		cout << "06. Save the KDTree to a binary file \n";
		cout << "07. Read The KDTree from a binary file\n";
		cout << "08. Print the KDTree in level order\n";
		cout << "09. Delete all nodes in the KDTree\n";
		cout << "10. Delete all information in the binary file\n";
		cout << "Input choose: ";
		int choose; cin >> choose;
		system("cls");
		if (choose == 0)
		{
			deleteAllNodes(root);
			break;
		}
		else if (choose == 1)
		{
			cout << "Input CSV file: ";
			string fileName;
			cin >> fileName;
			city = readFile(fileName);
			buildKDTree(root, city);
			if (!city.empty())
			{
				cout << "Load a list of cities is successful\n";
			}
		}
		else if (choose == 2)
		{
			inputCity(root);
		}
		else if (choose == 3)
		{
			if (root == NULL)
			{
				cout << "Please, load a list of cities from a CSV file\n";
			}
			else
			{
				string file;
				cout << "Input CSV file path: "; cin >> file;
				insertMultipleCities(file, root);
				if (root != NULL)
				{
					cout << "Insert multiple new cities is successful\n";
				}
			}
		}
		else if (choose == 4)
		{
			string file = "NearestNeighbor.csv";
			printNearestNeighborSearch(root, file);
		}
		else if (choose == 5)
		{
			string fileName = "QueryCities.csv";
			printRangeSearch(root, fileName);
		}
		else if (choose == 6)
		{
			string fileName = "KDTree.bin";
			saveKDTree(root, fileName);
		}
		else if (choose == 7)
		{

			string fileName = "KDTree.bin";
			deleteAllNodes(root);
			root = readKDTree(fileName);
			if (root != NULL)
			{
				cout << "Read the KDTree from a binary file is successful\n";
			}
		}
		else if (choose == 8)
		{
			printLevelOrder(root);
		}
		else if (choose == 9)
		{
			string file = "KDTree.bin";
			deleteAllNodes(root);
			clearBinaryFile(file);
			if (!root) cout << "Delete all nodes is successful\n";
		}
		else if (choose == 10)
		{
			string file = "KDTree.bin";
			clearBinaryFile(file);
			cout << "Delete all information in the binary file is successful\n";
		}
		_getch();
	}
}
/*
NOTE:
- KDTree.bin : Save KDTree to a binary file
- QueryCities.csv: Save info all cities in rectangle
- NearestNeighbor.csv: save city info with nearest neighbor
*/
int main()
{
	interface();
	return 0;
}
