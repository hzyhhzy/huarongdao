// huarongdao1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <map>
#include <cstdint>
using namespace std;
const int H =5, W = 4;
const int maxShape4Num = 6;//小兵太多了步数一定少，而且小兵多了状态数暴增，所以限制小兵个数
const int minShape4Num = 2;
bool doublemove = true;//true一个子连续移动两次算作一步，false一格算一步

int main1();
int main2();

int main()
{
  main1();//穷举所有局面
  //main2();//求解特定局面
}

struct Key
{
  static const int keyN =26;// =log(2^64)/log(5)
  static const int keySize = H * W / keyN + 1;
  static int64_t pow5[keyN];

  static void initPow5()
  {
    int64_t t = 1;
    for (int i = 0; i < keyN; i++)
    {
      pow5[i] = t;
      t *= 5;
    }

  }

  int64_t keys[keySize];
  Key()
  {
    for (int i = 0; i < keySize; i++)
      keys[i] = 0;
  }
  void update(int place, int64_t x)
  {
    int group = place / keyN;
    place = place % keyN;
    keys[group] += x * pow5[place];
  }
  Key(const Key& other)
  {

    for (int i = 0; i < keySize; i++)
      keys[i] = other.keys[i];
  }
  friend bool operator<(const Key& a, const Key& b)
  {
    for (int i = 0; i < keySize; i++)

    {
      if(a.keys[i]<b.keys[i])return true;
      else if(a.keys[i]>b.keys[i])return false;
    }
    return false;

  }
  friend std::ostream& operator<<(std::ostream& os, const Key& key)
  {
    for (int i = 0; i < keySize; i++)
    {
      os << key.keys[i];
      if (i != keySize - 1)os << "_";
    }
    return os;
  }
};
int64_t Key::pow5[keyN];

struct alignas(4) Block
{
  //0 空，1 2x2的那块，2 1x2，3 2x1，4 1x1
  int16_t shape;
  int8_t x, y;
  Block() :shape(0), x(0), y(0) {}
  Block(int16_t shape, int8_t x, int8_t y) :shape(shape), x(x), y(y) {}
};
class Board
{
  //static_assert(H <=6);
  //static_assert(W == 4);
public:
  //0 空，1 2x2的那块，2 1x2，3 2x1，4 1x1
  int8_t board[H * W];
  int8_t blocknum;
  Block blocks[H * W - 4];

  Key key;

  int minmove;//最少多少步能解

  Board() {
    for (int i = 0; i < H * W; i++)board[i] = 0;
    key = Key();
    blocknum = 0;
    minmove = -1;
  }
  //friend bool operator<(const Board& a,const Board& b) { return a.key < b.key; }
  void placeBlock(Block block)
  {
    blocks[blocknum] = block;
    blocknum++;
    int loc = block.x + block.y * W;
    key.update(loc,block.shape);
    if (block.shape == 1)board[loc] = board[loc + 1] = board[loc + W] = board[loc + W + 1] = block.shape;
    else if (block.shape == 2)board[loc] = board[loc + 1] = block.shape;
    else if (block.shape == 3)board[loc] = board[loc + W] = block.shape;
    else if (block.shape == 4)board[loc] = block.shape;

    if (block.shape == 1)
    {
      if ((block.x == W/2-1) && (block.y == H - 2))//finished
        minmove = 0;
    }

  }

  bool isPlaceBlockLegal(Block block)
  {
    int loc = block.y * W + block.x;
    if (board[loc] != 0)return false;
    if ((block.shape == 1 || block.shape == 2) && (block.x == W - 1 || board[loc + 1] != 0))return false;
    if ((block.shape == 1 || block.shape == 3) && (block.y == H - 1 || board[loc + W] != 0))return false;
    if ((block.shape == 1) && (board[loc + W + 1] != 0))return false;
    return true;
  }
  //bool isFinished() { return (blocks[0].x == 1) && (blocks[0].y == H - 2); }
  bool isLegal(int blockID, int dir)
  {
    //dir依次是上右下左
    int16_t shape = blocks[blockID].shape;
    int8_t x = blocks[blockID].x, y = blocks[blockID].y;
    int loc = x + y * W;
    if (dir == 0)
    {
      if (y == 0)return false;
      if (shape == 1 || shape == 2)
      {
        if (board[loc - W] != 0 || board[loc - W + 1] != 0)return false;
      }
      else
      {
        if (board[loc - W] != 0)return false;
      }
    }
    else if (dir == 1)
    {
      if (x == W - 1)return false;
      if ((shape == 1 || shape == 2) && x == W - 2)return false;
      if (shape == 1)
      {
        if (board[loc + 2] != 0 || board[loc + W + 2] != 0)return false;
      }
      else if (shape == 2)
      {
        if (board[loc + 2] != 0)return false;
      }
      else if (shape == 3)
      {
        if (board[loc + 1] != 0 || board[loc + W + 1] != 0)return false;
      }
      else
      {
        if (board[loc + 1] != 0)return false;
      }
    }
    else if (dir == 2)
    {
      if (y == H - 1)return false;
      if ((shape == 1 || shape == 3) && y == H - 2)return false;
      if (shape == 1)
      {
        if (board[loc + W + W] != 0 || board[loc + W + W + 1] != 0)return false;
      }
      else if (shape == 2)
      {
        if (board[loc + W] != 0 || board[loc + W + 1] != 0)return false;
      }
      else if (shape == 3)
      {
        if (board[loc + W + W] != 0)return false;
      }
      else
      {
        if (board[loc + W] != 0)return false;
      }
    }
    else if (dir == 3)
    {
      if (x == 0)return false;
      if (shape == 1 || shape == 3)
      {
        if (board[loc - 1] != 0 || board[loc + W - 1] != 0)return false;
      }
      else
      {
        if (board[loc - 1] != 0)return false;
      }
    }
    return true;
  }
  void play(int blockID, int dir)
  {

    int16_t shape = blocks[blockID].shape;
    int8_t x = blocks[blockID].x, y = blocks[blockID].y;
    int loc = x + y * W;
    if (dir == 0)
    {
      blocks[blockID].y--;
    }
    else if (dir == 1)
    {
      blocks[blockID].x++;
    }
    else if (dir == 2)
    {
      blocks[blockID].y++;
    }
    else if (dir == 3)
    {
      blocks[blockID].x--;
    }
    int newloc = blocks[blockID].x + blocks[blockID].y * W;

    key.update(loc,-shape);
    key.update(newloc, shape);



    if (shape == 1)
    {
      if (dir == 0)
      {
        board[loc - W] = shape;
        board[loc - W + 1] = shape;
        board[loc + W] = 0;
        board[loc + W + 1] = 0;
      }
      else if (dir == 1)
      {
        board[loc + 2] = shape;
        board[loc + W + 2] = shape;
        board[loc] = 0;
        board[loc + W] = 0;

      }
      else if (dir == 2)
      {
        board[loc + W + W] = shape;
        board[loc + W + W + 1] = shape;
        board[loc] = 0;
        board[loc + 1] = 0;

      }
      else if (dir == 3)
      {
        board[loc - 1] = shape;
        board[loc + W - 1] = shape;
        board[loc + 1] = 0;
        board[loc + W + 1] = 0;

      }
    }
    else if (shape == 2)
    {
      if (dir == 0)
      {
        board[loc - W] = shape;
        board[loc - W + 1] = shape;
        board[loc] = 0;
        board[loc + 1] = 0;

      }
      else if (dir == 1)
      {
        board[loc + 2] = shape;
        board[loc] = 0;

      }
      else if (dir == 2)
      {
        board[loc + W] = shape;
        board[loc + W + 1] = shape;
        board[loc] = 0;
        board[loc + 1] = 0;

      }
      else if (dir == 3)
      {
        board[loc - 1] = shape;
        board[loc + 1] = 0;

      }

    }
    else if (shape == 3)
    {

      if (dir == 0)
      {
        board[loc - W] = shape;
        board[loc + W] = 0;

      }
      else if (dir == 1)
      {
        board[loc + 1] = shape;
        board[loc + W + 1] = shape;
        board[loc] = 0;
        board[loc + W] = 0;


      }
      else if (dir == 2)
      {
        board[loc + W + W] = shape;
        board[loc] = 0;

      }
      else if (dir == 3)
      {
        board[loc - 1] = shape;
        board[loc + W - 1] = shape;
        board[loc] = 0;
        board[loc + W] = 0;

      }
    }
    else if (shape == 4)
    {

      board[loc] = 0;
      if (dir == 0)
      {
        board[loc - W] = shape;
      }
      else if (dir == 1)
      {
        board[loc + 1] = shape;

      }
      else if (dir == 2)
      {
        board[loc + W] = shape;

      }
      else if (dir == 3)
      {
        board[loc - 1] = shape;

      }

    }
  }
  void print()
  {
    for (int y = 0; y < H; y++)
    {
      for (int x = 0; x < W; x++)
      {
        int8_t c = board[x + y * W];
        if (c == 0)cout << "_ ";
        else if (c == 1)cout << "▓ ";
        else if (c == 2)cout << "▅ ";
        else if (c == 3)cout << "▋ ";
        else if (c == 4)cout << "■";
      }
      cout << endl;
    }
    cout << endl;
      
  }
  static void generateAllBoard(int* shapenum, int startLoc, const Board& baseboard, map<Key, Board>& allboard)
  {
    int shape = 0;
    bool lastBlockOfShape = false;
    bool lastBlock = false;
    for (int i = 0; i < 4; i++)
    {
      if (shapenum[i] != 0)
      {
        shape = i + 1;
        break;
      }
    }
    int shapesum = 0;
    for (int i = 0; i < 4; i++)
    {
      shapesum += shapenum[i];
    }
    lastBlockOfShape = (shapenum[shape - 1] == 1);
    lastBlock = shapesum == 1;
    for (int loc = startLoc; loc < H * W; loc++)
    {
      Board b = baseboard;
      Block block(shape, loc % W, loc / W);
      if (!b.isPlaceBlockLegal(block))continue;
      b.placeBlock(block);

      if (lastBlock)
      {
        allboard.insert(pair(b.key, b));
      }

      else
      {
        int nextStartLoc = lastBlockOfShape ? 0 : loc + 1;
        shapenum[shape - 1]--;
        generateAllBoard(shapenum, nextStartLoc, b, allboard);
        shapenum[shape - 1]++;
      }

    }
  }
};
int main2()//solve
{
  Key::initPow5();

  Board tosolveBoard;


  //267，6x4的最大步数

  int shapenum[4] = { 1,2,5,4 };
  tosolveBoard.placeBlock(Block(1, 1, 0));
  tosolveBoard.placeBlock(Block(2, 1, 2));
  tosolveBoard.placeBlock(Block(2, 1, 3));
  tosolveBoard.placeBlock(Block(3, 0, 2));
  tosolveBoard.placeBlock(Block(3, 3, 2));
  tosolveBoard.placeBlock(Block(3, 0, 4));
  tosolveBoard.placeBlock(Block(3, 1, 4));
  tosolveBoard.placeBlock(Block(3, 2, 4));
  tosolveBoard.placeBlock(Block(4, 0, 1));
  tosolveBoard.placeBlock(Block(4, 3, 0));
  tosolveBoard.placeBlock(Block(4, 3, 1));
  tosolveBoard.placeBlock(Block(4, 3, 4));


  //横刀立马
  /*
  int shapenum[4] = { 1,1,4,4 };
  tosolveBoard.placeBlock(Block(1, 1, 0));
  tosolveBoard.placeBlock(Block(2,1 , 2));
  tosolveBoard.placeBlock(Block(3,0 , 0));
  tosolveBoard.placeBlock(Block(3,3 ,0 ));
  tosolveBoard.placeBlock(Block(3,0 ,2 ));
  tosolveBoard.placeBlock(Block(3,3 ,2 ));
  tosolveBoard.placeBlock(Block(4,0 ,4 ));
  tosolveBoard.placeBlock(Block(4,1 ,3 ));
  tosolveBoard.placeBlock(Block(4,2 ,3 ));
  tosolveBoard.placeBlock(Block(4,3 ,4 ));
  */

  Key tosolveKey = tosolveBoard.key;




  map<Key, Board> allboard;
  cout << "ShapeNum=" << shapenum[1] << "," << shapenum[2] << "," << shapenum[3] << " ";
  Board::generateAllBoard(shapenum, 0, Board(), allboard);
  cout << "BoardNum=" << allboard.size() << " ";

  Key longestKey;//最长的局面(之一)的key

  for (int step = 0;; step++)
  {
    bool willfinish = true;//如果没有找到这么多步的局面就终止

    for (auto i = allboard.begin(); i != allboard.end(); i++)
    {
      Board& board = i->second;
      if (board.minmove != step)continue;

      for (int blockID = 0; blockID < board.blocknum; blockID++)
      {
        for (int dir1 = 0; dir1 < 4; dir1++)
          for (int dir2 = 0; dir2 < 4; dir2++)
          {
            bool oneMove = ((dir1 % 2 == dir2 % 2) && (dir1 != dir2));//正反抵消，无意义。所以刚好可以用来表示单步
            if (!doublemove && !oneMove)continue;
            if (!board.isLegal(blockID, dir1))continue;
            Board tmpboard = board;
            tmpboard.play(blockID, dir1);
            if (!oneMove)
            {
              if (!tmpboard.isLegal(blockID, dir2))continue;
              tmpboard.play(blockID, dir2);

            }
            //if (!allboard.count(tmpboard.key))cout << "error";
            Board& targetBoard = allboard[tmpboard.key];
            if (targetBoard.minmove == -1)
            {
              willfinish = false;
              longestKey = tmpboard.key;
              targetBoard.minmove = step + 1;

            }
          }
      }

    }
    if (willfinish)
    {
      cout << "MaxStep=" << step << "  Key=" << longestKey << endl;
      break;
    }
  }


  int movenum = allboard[tosolveKey].minmove;
  cout << "Solution=" << movenum << endl;

  Board cur_board = allboard[tosolveKey];
  cout << 0 << endl;
  cur_board.print();
  for (int i = 1; i <= movenum; i++)
  {
    bool foundSolution = false;
    for (int blockID = 0; blockID < cur_board.blocknum; blockID++)
    {
      if (foundSolution)break;
      for (int dir1 = 0; dir1 < 4; dir1++)
      {
        if (foundSolution)break;
        for (int dir2 = 0; dir2 < 5; dir2++)
        {
          if (foundSolution)break;
          if (!doublemove && dir2 != 4)continue;
          if (!cur_board.isLegal(blockID, dir1))continue;
          Board tmpboard = cur_board;
          tmpboard.play(blockID, dir1);
          if (dir2 != 4)
          {
            if (!tmpboard.isLegal(blockID, dir2))continue;
            tmpboard.play(blockID, dir2);

          }
          if (!allboard.count(tmpboard.key))cout << "error";
          Board& targetBoard = allboard[tmpboard.key];
          if (targetBoard.minmove == movenum-i)
          {
            cur_board = targetBoard;
            foundSolution = true;
            cout << i << endl;
            cur_board.print();
          }
        }
      }
    }

  }




  return 0;

}
int main1()
{
  Key::initPow5();

  //tosolveKey = 0;


  int maxTwoBlock = (H * W - 4 - 2) / 2;//最多有多少个两格block
  for (int shape2 = 0; shape2 <= maxTwoBlock; shape2++)
    for (int shape3 = 0; shape3 <= maxTwoBlock-shape2; shape3++)
  {
      int shape4 = (H * W - 4 - 2)  - 2 * shape2 - 2 * shape3;
      if (shape4 > maxShape4Num||shape4 < minShape4Num)continue;
    map<Key, Board> allboard;
    int shapenum[4] = { 1,shape2,shape3,shape4 };
    cout << "ShapeNum=" << shapenum[1] << "," << shapenum[2] << "," << shapenum[3] << " ";
    Board::generateAllBoard(shapenum, 0, Board(), allboard);
    cout << "BoardNum=" << allboard.size() << " ";

    Key longestKey;//最长的局面(之一)的key

    for (int step = 0;; step++)
    {
      bool willfinish = true;//如果没有找到这么多步的局面就终止

      for (auto i = allboard.begin(); i != allboard.end(); i++)
      {
        Board& board = i->second;
        if (board.minmove != step)continue;

        for (int blockID = 0; blockID < board.blocknum; blockID++)
        {
          for (int dir1 = 0; dir1 < 4; dir1++)
            for (int dir2 = 0; dir2 < 5; dir2++)
          {
              if (!doublemove && dir2 != 4)continue;
            if (!board.isLegal(blockID, dir1))continue;
            Board tmpboard = board;
            tmpboard.play(blockID, dir1);
            if (dir2 != 4)
            {
              if (!tmpboard.isLegal(blockID, dir2))continue;
              tmpboard.play(blockID, dir2);
              
            }
            if (!allboard.count(tmpboard.key))cout << "error";
            Board& targetBoard = allboard[tmpboard.key];
            if (targetBoard.minmove == -1)
            {
              willfinish = false;
              longestKey = tmpboard.key;
              targetBoard.minmove = step + 1;

            }
          }
        }

      }
      if (willfinish)
      {
        int noSolution = 0;
        for (auto i = allboard.begin(); i != allboard.end(); i++)
        {
          if (i->second.minmove == -1)noSolution++;
        }
        cout << "NoSolutionNum="<<noSolution<<"  MaxStep = " << step << "  Key = " << longestKey <<endl;
        allboard[longestKey].print();
        break;
      }
    }
  }




  return 0;

}




// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
