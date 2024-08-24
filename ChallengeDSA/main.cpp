#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<algorithm>
using namespace std;
#define pi 3.14
struct City {
	string name;
	double lat; // latitude
	double lng; // longitude
};
double stod_(string s) {
	double res = 0;
	int i = 0;
	bool isNegative = false;

	if (s[i] == '-') {
		isNegative = true;
		i++;
	}

	while (i < s.size() && s[i] != '.') {
		res = res * 10 + (s[i] - '0');
		++i;
	}

	if (i < s.size() && s[i] == '.') {
		++i; // skip '.'
		double frac = 0, factor = 1;
		while (i < s.size()) {
			factor /= 10;
			frac += (s[i] - '0') * factor;
			++i;
		}
		res += frac;
	}

	return isNegative ? -res : res;
}

vector<City> readFile(string filename) {
	vector<City> data;
	string read;
	City tmp_city;
	ifstream f(filename.c_str());

	if (!f.is_open())
		return data;

	getline(f, read);

	while (getline(f, read)) {
		string tmp;
		stringstream ss(read);

		getline(ss, tmp_city.name, ','); // read city's name

		getline(ss, tmp, ','); // read latitude
		tmp_city.lat = stod_(tmp);

		getline(ss, tmp, ','); // read longitude
		tmp_city.lng = stod_(tmp);

		data.push_back(tmp_city);
	}

	return data;
}
void Print(vector<City> data) {
	for (int i = 0; i < 100; i++) {
		cout << data[i].name << "; "
			<< setprecision(5) << fixed << data[i].lat << "; " << setprecision(5) << fixed
			<< data[i].lng << "\n";
	}
}
struct Node
{
	Node* left;
	Node* right;
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
void Insert(Node*& root, City city, int depth = 0)
{
	if (root == NULL)
	{
		Node* newNode = createNode(city);
		root = newNode;
		return;
	}
	int k = depth % 2;
	if (k == 0)
	{
		if (city.lat < root->key.lat)
			Insert(root->left, city, depth + 1);
		else if (city.lat >= root->key.lat)
			Insert(root->right, city, depth + 1);
		
	}
	else
	{
		if (city.lng < root->key.lng)
			Insert(root->left, city, depth + 1);
		else if (city.lng >= root->key.lng)
			Insert(root->right, city, depth + 1);
	}
}
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
bool isInRectangle(City city, double posRectangel1[2], double posRectangel2[2])
{
	if (city.lat < min(posRectangel1[0], posRectangel2[0]) || city.lat > max(posRectangel1[0], posRectangel2[0]))
		return false;
	if (city.lng < min(posRectangel1[1], posRectangel2[1]) || city.lng > max(posRectangel1[1], posRectangel2[1]))
		return false;
	return true;
}
void RangeSearch(Node* root, vector<City>& city, double posRectangel1[2], double posRectangel2[2], int depth = 0)
{
	if (root == NULL)
		return;
	int k = depth % 2;
	if (k == 0)
	{
		if (root->key.lat < min(posRectangel1[k], posRectangel2[k]))
		{
			RangeSearch(root->right, city, posRectangel1, posRectangel2, depth + 1);
		}
		else if (root->key.lat > max(posRectangel1[k], posRectangel2[k]))
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
	else if(k == 1)
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
void NearestNeighborSearch(Node* root, City& city, double pos[2], double& bestDistance, int depth = 0)
{
	if (root == NULL)
		return;
	int k = depth % 2;
	double d = distance(root->key, pos);
	if (d < bestDistance)
	{
		bestDistance = d;
		city = root->key;
	}
	if (k == 0)
	{
		if (pos[k] < root->key.lat)
		{
			NearestNeighborSearch(root->left, city, pos, bestDistance, depth + 1);
			double diff = 2 * 6371.0 * asin(sin((fabs((pos[k] - root->key.lat)) * pi / 180) / 2));
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
void buildKDTree(Node*& root, vector<City>v, int depth = 0)
{
	if (v.empty())
		return;
	int k = depth % 2;
	if (k == 0)
		sort(v.begin(), v.end(), compareLat);
	else
		sort(v.begin(), v.end(), compareLng);
	int median = v.size() / 2;
	
	root = createNode(v[median]);

	vector<City> leftCity(v.begin(), v.begin() + median);
	vector<City> rightCity(v.begin() + median + 1, v.end());

	buildKDTree(root->left, leftCity, depth + 1);
	buildKDTree(root->right, rightCity, depth + 1);
}
void NLR(Node* root)
{
	if (root == NULL)
		return;
	cout << root->key.name << endl;
	NLR(root->left);
	NLR(root->right);
}
int main()
{
	/*vector<City>v = readFile("worldcities-20210313-population-50000+.csv");
	Node* root = NULL;
	for (int i = 0; i < v.size(); i++)
		Insert(root, v[i]);
	double pos[2] = { 30, 35 };
	City city; double best = INT_MAX;
	NearestNeighborSearch(root, city, pos, best);
	cout << city.name << "," << city.lat << "," << city.lng << endl << endl;
	cout << distance(city, pos) << endl;
	double min = INT_MAX;
	for (int i = 0; i < v.size(); i++)
	{
		if (distance(v[i], pos) < min)
		{
			min = distance(v[i], pos);
		}
	}
	cout << min << endl;*/
	/*double pos1[2] = { -0.1,34.75 };
	double pos2[2] = { 30, 35 };
	cout << distance(pos1, pos2);*/
	/*vector<City>v = readFile("worldcities-20210313-population-50000+.csv");
	Node* root = NULL;
	buildKDTree(root, v);
	vector<City>res;
	double pos1[2] = { 35.6897,139.6923 };
	double pos2[2] = { 40.6943, -73.9249 };
	City city;
	double bestDistance = INT_MAX;
	NearestNeighborSearch(root, city, pos1, bestDistance);
	cout << bestDistance << "," << city.name;*/
}