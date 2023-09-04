#include<bits/stdc++.h>
#include<vector>
#include<utility>
using namespace std;
int main(int argc, char* argv[]){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int city;
    int point[100][2];
    char *filename = argv[1];             //儲存城市座標的檔案名稱(ex : .\\dt1\\point.txt)
    ifstream file;
    file.open(filename);                  //開檔
    if(!file.is_open()){                  //檔案無法開啟
        cout << "can not open " << filename << endl;
        return EXIT_FAILURE;
    }
    while(file >> city){                  
        file >> point[city][0] >> point[city][1];        //讀取每個城市的座標
    }
    float distance[city+1][city+1];
    file.close();
    for(int i=1;i<=city;i++){                  //計算每個城市和其他城市的距離
        for(int j=i+1;j<=city;j++){
            distance[i][j] = distance[j][i] = sqrt(pow(point[i][0] - point[j][0],2) + pow(point[i][1] - point[j][1],2));
        }
    }

    int len = (1<<(city-1));          //2^(n-1)
    int f, s;
    vector<pair<float, pair<int,int>>> dp[city+1];        //dis,previous_city,previous_index 
    float min, dis;
    dp[1].push_back(make_pair(0, make_pair(1,0)));
    for(int i=2;i<=city;i++){
        dp[i].push_back(make_pair(distance[i][1], make_pair(i,0)));
    }
    for(int i=1;i<len;i++){
        bitset<24> b(i);
        min = 3.40282e+038;
        for(int k=0;k<city;k++){         //city 1
            bitset<24> temp = b;
            if(b[k]){
                temp[k] = 0;                 //每次將其中一個城市設為0,並計算其他點的最短路徑
                int t = (int)temp.to_ulong();
                dis = distance[1][k+2] + dp[k+2][t].first;
                if(min > dis){
                    f = k+2;
                    s = t;
                    min = dis;
                }
            }
        }
        dp[1].push_back(make_pair(min,make_pair(f, s)));
        for(int j=2;j<=city;j++){          //city2 ~ j
            min = 3.40282e+038;
            if(b[j-2] == 1){               //已走過該點,不須再計算
                dp[j].push_back(make_pair(0, make_pair(0,0)));
                continue;
            }
            for(int k=0;k<city;k++){
                bitset<24> temp = b;
                if(b[k]){
                    temp[k] = 0;
                    int t = (int)temp.to_ulong();
                    dis = distance[j][k+2] + dp[k+2][t].first;
                    if(min > dis){
                        f = k+2;
                        s = t;
                        min = dis;
                    }
                }
            }
            dp[j].push_back(make_pair(min,make_pair(f, s)));
        }
    }

    ofstream file1;
    string str(argv[1]);
    file1.open(str.substr(0,str.length()-string("point.txt").length()) + "ans_" + str.substr(str.find("dt"),3) + ".txt");  //創建新的txt檔
    file1 << "distance: " << dp[1][len-1].first << endl;
    int c = dp[1][len-1].second.first,index = dp[1][len-1].second.second;
    file1 << 1 << endl;
    for(int i=0;i<city-1;i++){
        file1 << c << endl;
        pair<int,int> p = dp[c][index].second;
        c = p.first;
        index = p.second;
    }
    file1.close();
    return 0;
}