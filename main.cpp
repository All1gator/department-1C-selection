#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <map>
#include <stack>
#include <set>
#include <fstream>
#include <random>
#include <ctime>
#include <iomanip>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;

#define all(x) x.begin(), x.end()
//#define int long long
ll mod = 1e9 + 7;
//ll mod = 998244353;
//ll mod = 7340033;
ll inf = 1e18  * 2 + 7;
int INFI = 1e9 + 7;
mt19937 rn(time(NULL));

int counter_of_free_space = 0;
int counter_of_free_questions = 4;
ll ans = 0;
ll a, b, c;

struct ver{
  char name = '?';
  bool visited = false;
  ver* direct[4];

  ver(char new_name) {
    name = new_name;
    visited = false;
  }
};



ver spec('#');

class minotaur{
public:
  ver* ver_now;
  int dir = -1; // 0 вверх 1 вправа 2 вниз 3 влево

  void visit() {
    ver_now->visited = true;
  }

  void clockwise() {
    ++dir;
    if (dir == 4) {
      dir = 0;
    }
  }
  void counterclockwise() {
    --dir;
    if (dir == -1) {
      dir = 3;
    }
  }

  void go() {
    if (ver_now->direct[dir] == nullptr) {
      ver_now->direct[dir] = new ver('_');
    }
    ver_now = ver_now->direct[dir];
  }

  minotaur(int x, int y, int x1, int y1) {
    if (x1 < x) {
      dir = 3;
    }
    if (x1 > x) {
      dir = 1;
    }
    if (y1 < y) {
      dir = 2;
    }
    if (y1 > y) {
      dir = 0;
    }
    ver_now = new ver('_');
    this->visit();
    for (int i = 0; i < 4; ++i) {
      ver_now->direct[i] = new ver('?');
    }
  }

};

struct Line{
  double a, b, c;

  Line(double x1,double y1,double x2,double y2) {
    if (x1 == x2) {
      a = 0;
      b = 1;
      c = -x1;
    }
    double k = (y1 - y2) / (x1 - x2);
    double b = y1 - x1 * b;
    a = -k;
    b = 1;
    c = -b;
  }

  double calc(ll x, ll y) {
    return a * x + b * y + c;
  }
};

struct point{
  int x, y;
  point(int n_x, int  n_y) {
    x = n_x;
    y = n_y;
  }
  point() = default;
};

point operator+(point a, point b) {
    return point(a.x + b.x, a.y + b.y);
}

bool operator==(point a, point b) {
    return (a.x == b.x && a.y == b.y);
}

bool operator!=(point a, point b) {
    return !(a == b);
}

point operator-(point a, point b) {
    return point(a.x - b.x, a.y - b.y);
}

bool operator<(point a, point b) {
    return (a.x < b.x && a.y < b.y);
}

int new_dir(int now, int add) {
  now += add;
  if (now == -1) {
    now = 3;
  }
  if (now == 4) {
    now = 0;
  }
  return now;
}

ll N = 1e4;
vector<point> directions(4);
vector<vector<char> > field(N, vector<char> (N, '?'));
vector<vector<bool> > visited(N, vector<bool> (N));

int NEW_X;
int NEW_Y;

vector<pair<int, int> > find_ver(int x, int y, int dir, char name) {
  set<pair<int, pair<int, pair<int, int> > > > q;

  q.insert({1, {x, {y, dir}}});
  int ans_x;
  int ans_y;
  map<pair<int, pair<int, int> >, int> cost;
  map<pair<int, pair<int, int> >, pair<int, pair<int, int> > > prev;
  cost[{dir,{x,y}}] = 1;
  prev[{dir,{x,y}}] = {-1, {0,0}};
  pair<int, pair<int, int>> ans;
  while (true) {
    //cout << "!!!" << endl;
    //cout << q.size() << endl;
    auto now = *q.begin();
    q.erase(*q.begin());
    //cout << "LOOK" << endl;
    //cout << now.first << ' ' << now.second.first << ' '  << now.second.second.first << ' ' << now.second.second.second << endl;
    //cout << directions[now.second.second.second].x << ' '<< directions[now.second.second.second].y << endl;
    if (field[now.second.first][now.second.second.first] == name && !visited[now.second.first][now.second.second.first]) {
      ans = {now.second.second.second, {now.second.first, now.second.second.first}};
      NEW_X = now.second.first;
      NEW_Y = now.second.second.first;
      break;
    }
    //cout << "END" << endl;
    int now_dir = now.second.second.second;
    point now_pos = point(now.second.first, now.second.second.first);
    //cout << now_dir << ' ' << now_pos.x << ' ' << now_pos.y << endl;
    if (field[now_pos.x][now_pos.y] != '_') {
      continue;
    }
    int new_cost = now.first + a;
    //cout << new_cost << ' '  << now_dir << ' ' << now_pos.x << ' ' << now_pos.y << endl;
    if (cost[{now_dir,{now_pos.x + directions[now_dir].x, now_pos.y + directions[now_dir].y}}] == 0
        || cost[{now_dir,{now_pos.x + directions[now_dir].x, now_pos.y + directions[now_dir].y}}] > new_cost) {
      q.insert({new_cost, {now_pos.x + directions[now_dir].x, {now_pos.y + directions[now_dir].y, now_dir}}});
      prev[{now_dir,{now_pos.x + directions[now_dir].x, now_pos.y + directions[now_dir].y}}] = {now_dir,{now_pos.x, now_pos.y}};
      cost[{now_dir,{now_pos.x + directions[now_dir].x, now_pos.y + directions[now_dir].y}}] = new_cost;
      //cout << "LOOK" << ' ' << now_dir << ' ' << now_pos.x << ' ' << now_pos.y << endl;
    }
    new_cost -= a;
    new_cost += b;
    int  old_dir = now_dir;
    now_dir = new_dir(now_dir, 1);
    if (cost[{now_dir,{now_pos.x, now_pos.y}}] == 0 || cost[{now_dir,{now_pos.x, now_pos.y}}] > new_cost) {
      q.insert({new_cost, {now_pos.x , {now_pos.y , now_dir}}});
      prev[{now_dir,{now_pos.x, now_pos.y}}] = {old_dir,{now_pos.x, now_pos.y}};
      cost[{now_dir,{now_pos.x, now_pos.y}}] = new_cost;

    }
    now_dir = new_dir(now_dir, -1);
    now_dir = new_dir(now_dir, -1);
    if (cost[{now_dir,{now_pos.x, now_pos.y}}] == 0 || cost[{now_dir,{now_pos.x, now_pos.y}}] > new_cost) {
      q.insert({new_cost, {now_pos.x, {now_pos.y , now_dir}}});
      prev[{now_dir,{now_pos.x, now_pos.y}}] = {old_dir,{now_pos.x, now_pos.y}};
      cost[{now_dir,{now_pos.x, now_pos.y}}] = new_cost;

    }
  }
  //cout << "CHECK" << endl;
  vector<pair<int, int> > answer;
  //cout << ans.first << ' ' << ans.second.first <<  ' ' << ans.second.second << endl;
  while (ans.first != -1) {
    //cout << ans.first << endl;
    auto now = prev[ans];
    //cout << now.first << ' ' << now.second.first <<  ' ' << now.second.second << endl;
    if (now.first == -1) {
      break;
    }
    if (ans.first != now.first) {
      if (new_dir(now.first, 1) == ans.first) {
        answer.push_back({2, 1});
      } else {
        answer.push_back({2, 0});
      }
    } else {
      answer.push_back({1, 1});
    }
    ans = prev[ans];
  }
  reverse(all(answer));
  return answer;
}

void run() {
  //srand(time(NULL));
  int x,y,x1,y1, k;
  cin >> x >> y >> x1 >> y1 >> a >> b >> c >> k;
  directions[0] = point(-1,0);
  directions[1] = point(0,1);
  directions[2] = point(1,0);
  directions[3] = point(0,-1);
  ll D = 4 * a + 3 * b + c * k / 3;
  ll ver_second_actions = 4 * a + 3 * b;
  ll ver_first_action = c * k / 3;

  x += N / 2;
  y += N / 2;
  x1 += N / 2;
  y1 += N / 2;

  map<point, int> mp;
  for (int i = 0; i < 4; ++i) {
    mp[directions[i]] = i;
  }
  int dir = -1; // 0 вверх 1 вправо 2 вниз 3 влево
  if (x1 < x) {
    dir = 3;
  }
  if (x1 > x) {
    dir = 1;
  }
  if (y1 < y) {
    dir = 2;
  }
  if (y1 > y) {
    dir = 0;
  }

  visited[x][y] = true;
  field[x][y] = '_';
  while (counter_of_free_space > 0 || counter_of_free_questions > 0) {
    bool yes = 1;
    //если вершины вокруг  стены или посещены
    for (int i = 0; i < 4; ++i) {
      if (!(field[x + directions[i].x][y + directions[i].y] == '#' || visited[x + directions[i].x][y + directions[i].y])) {
        yes = 0;
      }
    }
    if (yes) {//find_questions
      if (counter_of_free_questions > 0) {
        vector<pair<int, int> > answer = find_ver(x, y, dir, '?');
        for (int i = 0; i < answer.size() - 1; ++i) {
          auto v = answer[i];
          if (v.first == 1) {
            ans += a;
            cout << 1 << endl;
            int x;
            cin >> x;
          } else {
            ans += b;
            cout << v.first << ' ' << v.second << endl;
            int x;
            cin >> x;
          }
        }
      }
    }
    ll ran = (rn()) % D;
    //cout << ran << ' ' << D << ' ' << ver_first_action << ' ' << ver_second_actions << endl;
    if (ran < ver_second_actions) { // осматриваемся
      cout << 3 << endl;
      ans += c;
      vector<vector<char>> checker(2 * k + 1, vector<char> (2 * k + 1));
      for (int i = 0; i < 2 * k + 1; ++i) {
        for (int j = 2 * k; j >= 0; --j) {
          cin >> checker[j][i];
        }
      }
      for (int i = 0; i < k; ++i) {
      swap(checker[i], checker[2 * k - i]);
      }
      for (int i = 0; i < 2 * k + 1; ++i) {
        for (int j = i; j < 2 * k + 1; ++j) {
          swap(checker[i][j], checker[j][i]);
        }
      }
      //cout << "!!!!" << endl;
      for (int i = x; i < x + k + 1; ++i) {
        for (int j = y - k; j < y + k + 1; ++j) {
          //cout << i << ' ' << j << endl;
          if (checker[i - x + k][j - y + k] == '#') {
            bool yes = 1;
            Line now_line = Line(x, y, i,j);
            //cout << i << ' ' << j << ' ' << x << ' ' << y << endl;
            for (int h = min(i, x); h <= max(i,x); ++h) {
              for (int g = min(j, y); g <= max(j,y); ++g) {
                if (checker[h - x + k][h - y + k] == '#') {
                  double now1 = now_line.calc(h + 0.5, g + 0.5);
                  double now2 = now_line.calc(h + 0.5, g - 0.5);
                  double now3 = now_line.calc(h - 0.5, g - 0.5);
                  double now4 = now_line.calc(h - 0.5, g + 0.5);
                  vector<double> cnt_now = {now1, now2, now3, now4};
                  sort(all(cnt_now));
                  if (cnt_now[0] < 0 && cnt_now[3] > 0) {
                    yes = 0;
                    break;
                  }
                }
              }
            }
            //cout << i << ' ' << j << "!!!" << endl;
            if (!yes) {
              checker[i - x + k][j - y + k] = '?';
            }
          }
        }
      }
      //cout << "!!!!!!!" << endl;
      for (int i = x; i > x - k - 1; --i) {
        for (int j = y - k; j < y + k + 1; ++j) {
          if (checker[i - x + k][j - y + k] == '#') {
            bool yes = 1;
            Line now_line = Line(x, y, i,j);
            for (int h = min(i, x); h <= max(i,x); ++h) {
              for (int g = min(j, y); g <= max(j,y); ++g) {
                if (checker[h - x + k][h - y + k] == '#') {
                  double now1 = now_line.calc(h + 0.5, g + 0.5);
                  double now2 = now_line.calc(h + 0.5, g - 0.5);
                  double now3 = now_line.calc(h - 0.5, g - 0.5);
                  double now4 = now_line.calc(h - 0.5, g + 0.5);
                  vector<double> cnt_now = {now1, now2, now3, now4};
                  sort(all(cnt_now));
                  if (cnt_now[0] < 0 && cnt_now[3] > 0) {
                    yes = 0;
                    break;
                  }
                }
              }
            }
            if (!yes) {
              checker[i - x + k][j - y + k] = '?';
            }
          }
        }
      }
      //cout << "!!!!" << endl;
      for (int i = 0; i < 2 * k + 1; ++i) {
        for (int j = 0; j < 2 * k + 1; ++j) {
          if (field[x + i - k][y + j - k] == '?') {
            //cout << checker[i][j];
            if (checker[i][j] == '#') {
              field[x + i - k][y + j - k] = '#';
              counter_of_free_questions--;
            }
            if (checker[i][j] == '_') {
              field[x + i - k][y + j - k] = '_';
              counter_of_free_questions--;
              counter_of_free_space++;
            }
          }
        }
        //cout << endl;
      }
      //cout << "LOOK" << endl;
      for (int i = x - 2; i < x + 2; i++) {
        for (int j = y - 2; j < y + 2; j++) {
          //cout << field[i][j];
        }
        //cout << endl;
      }
    } else { // брутфорсимся
      int cnt = 0;
      point now = point(x, y);
      point now_dir = directions[dir];
      for (int i = 0; i < 5; ++i) {
        if (cnt >= 2) {
          break;
        }
        if (field[now.x + directions[dir].x][now.y + directions[dir].y] == '#' || visited[now.x + directions[dir].x][now.y + directions[dir].y]) {
          if (field[now.x + directions[(dir + 1) % 4].x][now.y + directions[(dir + 1) % 4].y] == '#' ||
               visited[now.x + directions[(dir + 1) % 4].x][now.y + directions[(dir + 1) % 4].y]) {
            ans += b;
            cout << 2 << ' ' << 0 << endl;
            int x;
            cin >> x;
            dir = new_dir(dir, 1);
            cnt++;
          } else {
            ans += b;
            cout << 2 << ' ' << 1 << endl;
            int x;
            cin >> x;
            dir = new_dir(dir, -1);
            cnt++;
          }
        } else {
          cout << 1 << endl;
          int x;
          cin >> x;

          if (x == 0) {
            field[now.x + directions[dir].x][now.y + directions[dir].y] = '#';
          } else {
            now = now + directions[dir];
            if (field[now.x][now.y] == '_' && !visited[now.x][now.y]) {
              counter_of_free_space--;
            }
            if (field[now.x][now.y] == '?') {
              counter_of_free_questions--;
            }
            visited[now.x][now.y] = true;
            field[now.x][now.y] = '_';
            cnt = 0;
          }
        }
      }
      x = now.x;
      y = now.y;
    }
    // теперь надо найти наиближайшую точку не покрашенную
    while (counter_of_free_space > 0) {
      //cout << "!!!" << endl;
      vector<pair<int, int> > answer = find_ver(x, y, dir, '_');
      //cout << "ALARM" << endl;
      for (int i = 0; i < answer.size(); ++i) {
        auto v = answer[i];
        if (v.first == 1) {
          ans += a;
          cout << 1 << endl;
          int x;
          cin >> x;
        } else {
          ans += b;
          cout << v.first << ' ' << v.second << endl;
          int x;
          cin >> x;
        }
      }
      x = NEW_X;
      y = NEW_Y;
      visited[x][y] = true;
      counter_of_free_space--;
    }

  }
  cout << 4 << ' ' << ans << endl;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    //cin >> t;
    t = 1;
    while (t--) run();
    return 0;
}

/*
1
3
4 5 6
15
*/

