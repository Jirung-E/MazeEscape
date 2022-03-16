#include <iostream>
#include <random>
#include <stack>


class Cell {
  public:
    enum class Object {
        Wall = '#', Road = ' ', Goal = 'G', Player = 'P'
    };
    enum class Direction {
        Up, Right, Down, Left
    };
    
    // variables
    Object element[3][3];
    
    // constructor
    Cell() : element { } {
        for(int i=0; i<3; ++i) {
            for(int k=0; k<3; ++k) {
                element[i][k] = Object::Wall;
            }
        }
    }
    
    // functions
    void BreakWall(Direction direction) {
        element[1][1] = Object::Road;
        switch(direction) {
            case Direction::Up:
                element[0][1] = Object::Road;
                break;
            case Direction::Right:
                element[1][2] = Object::Road;
                break;
            case Direction::Down:
                element[2][1] = Object::Road;
                break;
            case Direction::Left:
                element[1][0] = Object::Road;
                break;
        }
        
        /*
        if(element[0][1] == Object::Road && element[1][2] == Object::Road) {
            element[0][2] = Object::Road;
        }
        if(element[1][2] == Object::Road && element[2][1] == Object::Road) {
            element[2][2] = Object::Road;
        }
        if(element[2][1] == Object::Road && element[1][0] == Object::Road) {
            element[2][0] = Object::Road;
        }
        if(element[1][0] == Object::Road && element[0][1] == Object::Road) {
            element[0][0] = Object::Road;
        }
        */
    }
    
    // Walls
    // 1)
    // ###
    //    
    //   
    //
    // 2)
    // ###
    // #  
    // #  
    // 
    // 3)
    // #  
    // #  
    // ###
    // .....
        
};

class Maze {                    // recursive backtracking
  public:
    // variables
    unsigned short width;
    unsigned short height;
    Cell** cell;
    
    //constructor
    Maze(unsigned short width, unsigned short height) : width { width }, height { height }, cell { nullptr } {
        if(width < 6) {
            this->width = 6;
        }
        else if(width > 12) {
            this->width = 12;
        }
        if(height < 6) {
            this->height = 6;
        }
        else if(height > 18) {
            this->height = 18;
        }
        
        cell = new Cell*[height];
        for(int i=0; i<height; ++i) {
            cell[i] = new Cell[width];
        }
        
        createMaze();
    }
    Maze() : Maze(6, 6) {
        
    }
    ~Maze() {
        for(int i=0; i<height; ++i) {
            delete[] cell[i];
        }
        delete[] cell;
    }
    
    // functions
    void Show() const {
        for(int i=0; i<height; ++i) {
            for(int n=0; n<3; ++n) {
                for(int k=0; k<width; ++k) {
                    for(int m=0; m<3; ++m) {
                        std::cout << static_cast<char>(cell[i][k].element[n][m]) << ' ';
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    
  private:
    Cell::Direction randomDirection() {
        std::random_device rd;
        std::mt19937 gen { rd() };
        std::uniform_int_distribution<int> uid { 0, 3 };
        
        switch(uid(gen)) {
            case 0:
                return Cell::Direction::Up;
            case 1:
                return Cell::Direction::Right;
            case 2:
                return Cell::Direction::Down;
            case 3:
                return Cell::Direction::Left;
        }
    }
    void createMaze() {
        using namespace std;
        
        struct Coord {
            unsigned short x = 0;
            unsigned short y = 0;
        } current;
        bool** visited = new bool*[height];
        for(int i=0; i<height; ++i) {
            visited[i] = new bool[width];
        }
        std::stack<Coord> stk;
        
        bool repeat = true;
        while(repeat) {
            // 방문했다고 체크
            if(visited[current.y][current.x] == false) {
                //cout << "visit check" << endl;
                visited[current.y][current.x] = true;
                stk.push(current);
            }
            
            // 갇혔는지
            bool istrapped[4] = { false, false, false, false };
            
            // 왼쪽 ㄱㄴ?
            if(current.x > 0) {
                if(visited[current.y][current.x-1]) {
                    istrapped[0] = true;
                }
                else {
                    istrapped[0] = false;
                }
            }
            else {
                istrapped[0] = true;
            }
            // 오른쪽 ㄱㄴ?
            if(current.x < width-1) {
                if(visited[current.y][current.x+1]) {
                    istrapped[1] = true;
                }
                else {
                    istrapped[1] = false;
                }
            }
            else {
                istrapped[1] = true;
            }
            // 위 ㄱㄴ?
            if(current.y > 0) {
                if(visited[current.y-1][current.x]) {
                    istrapped[2] = true;
                }
                else {
                    istrapped[2] = false;
                }
            }
            else {
                istrapped[2] = true;
            }
            // 아래 ㄱㄴ?
            if(current.y < height-1) {
                if(visited[current.y+1][current.x]) {
                    istrapped[3] = true;
                }
                else {
                    istrapped[3] = false;
                }
            }
            else {
                istrapped[3] = true;
            }
            
            // 갇힘
            if(istrapped[0]) {
                if(istrapped[1]) {
                    if(istrapped[2]) {
                        if(istrapped[3]) {
                            // 이전셀로 이동
                            if(stk.empty()) {
                                repeat = false;
                                break;
                            }
                            //std::cout << "back" << std::endl;
                            current = stk.top();
                            stk.pop();
                            continue;
                        }
                    }
                }
            }
                
            
            
            // 랜덤한 방향으로 이동
            switch(randomDirection()) {
                case Cell::Direction::Up:
                    if(current.y > 0 && visited[current.y-1][current.x] == false) {
                        cell[current.y][current.x].BreakWall(Cell::Direction::Up);
                        cell[current.y-1][current.x].BreakWall(Cell::Direction::Down);
                        current.y--;
                    }
                    break;
                case Cell::Direction::Right:
                    if(current.x < width-1 && visited[current.y][current.x+1] == false) {
                        cell[current.y][current.x].BreakWall(Cell::Direction::Right);
                        cell[current.y][current.x+1].BreakWall(Cell::Direction::Left);
                        current.x++;
                    }
                    break;
                case Cell::Direction::Down:
                    if(current.y < height-1 && visited[current.y+1][current.x] == false) {
                        cell[current.y][current.x].BreakWall(Cell::Direction::Down);
                        cell[current.y+1][current.x].BreakWall(Cell::Direction::Up);
                        current.y++;
                    }
                    break;
                case Cell::Direction::Left:
                    if(current.x > 0 && visited[current.y][current.x-1] == false) {
                        cell[current.y][current.x].BreakWall(Cell::Direction::Left);
                        cell[current.y][current.x-1].BreakWall(Cell::Direction::Right);
                        current.x--;
                    }
                    break;
            }
            
            repeat = false;
            for(int i=0; i<height; ++i) {
                for(int k=0; k<width; ++k) {
                    if(visited[i][k] == false) {
                        repeat = true;
                    }
                }
            }
            
            //Show();
        }
        
        for(int i=0; i<height; ++i) {
            delete[] visited[i];
        }
        delete[] visited;
    }
    
  public:
    void map(char** strmap) const {
        for(int i=0; i<height; ++i) {
            for(int n=0; n<3; ++n) {
                for(int k=0; k<width; ++k) {
                    for(int m=0; m<3; ++m) {
                        strmap[i*3+n][k*3+m] = static_cast<char>(cell[i][k].element[n][m]);
                        std::cout << strmap[i*3+n][k*3+m] << ' ';
                    }
                }
                strmap[i*3+n][width*3] = '\n';
                std::cout << std::endl;
            }
        }
        
        strmap[1][1] = 'P';
        strmap[height*3-2][width*3-2] = 'G';
        //Show();
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <iostream>
#include <chrono>
#include <thread>


class Object {
    char simbol;
    
  public:
    Object(char simbol) : simbol { simbol } {
        
    }
    
    Object operator=(const Object& obj) {
        this->simbol = obj.simbol;
        
        return *this;
    }
    
    virtual char Simbol() const {
        return simbol;
    }
};

class Player : public Object {
    
  public:
    Player() : Object { 'P' } {
        
    }
    
    char Simbol() const {
        return 'P';
    }
};

class Block : public Object {
  public:
    Block() : Object { '#' } {
        
    }
};

class Goal : public Object {
  public:
    Goal() : Object { 'G' } {
        
    }
};

class Map {
    size_t width;
    size_t height;
    Object*** map;
    
    enum class ObjectCode {
        Empty, Block, Goal, Player
    };
    
    class Player_ : public Player {
      public:
        enum class Direction {
            Right, Down, Left, Up
        } direc;
        
        class Position {
          public:
            unsigned short x;
            unsigned short y;
            
            Position() : x { 0 }, y { 0 } {
                
            }
            Position(unsigned short x, unsigned short y) : x { x }, y { y } {
                
            }
        } position;
        
        
        Player_() : Player(), direc { Direction::Right }, position { 0, 0 } {
            
        }
        
        Direction nextDirection() const {
            switch(direc) {
                case Direction::Right:
                    return Direction::Down;
                case Direction::Down:
                    return Direction::Left;
                case Direction::Left:
                    return Direction::Up;
                case Direction::Up:
                    return Direction::Right;
            }
        }
        
        Direction prevDirection() const {
            switch(direc) {
                case Direction::Right:
                    return Direction::Up;
                case Direction::Up:
                    return Direction::Left;
                case Direction::Left:
                    return Direction::Down;
                case Direction::Down:
                    return Direction::Right;
            }
        }
        
        /*bool Go() {
            unsigned short temp_x = position.x, temp_y = position.y;
            switch(direc) {
                case Direction::Down:
                    temp_y++;
                    PlayerGoDown();
                    break;
                case Direction::Up:
                    PlayerGoUp();
                    temp_y--;
                    break;
                case Direction::Left:
                    temp_x--;
                    PlayerGoLeft();
                    break;
                case Direction::Right:
                    temp_x++;
                    PlayerGoRight();
                    break;
            }
        }*/
    } player;
    
    
    void Initialize() {
        map[1][1] = new Player;
        player.position = { 1, 1 };
        map[0][0] = new Block;
        map[1][width-2] = new Goal;
        
        for(int i=0; i<width; ++i) {
            map[0][i] = new Block;
            map[height-1][i] = new Block;
        }
        for(int i=0; i<height; ++i) {
            map[i][0] = new Block;
            map[i][width-1] = new Block;
        }
    }
    void Initialize(std::string* strmap) {
        for(int i=0; i<height; ++i) {
            for(int k=0; k<width; ++k) {
                switch(strmap[i][k]) {
                    case '#':
                        map[i][k] = new Block;
                        break;
                    case 'P':
                        map[i][k] = new Player;
                        player.position = { k, i };
                        std::cout << player.position.x << ", " << player.position.y << std::endl;
                        break;
                    case 'G':
                        map[i][k] = new Goal;
                        break;
                    case ' ':
                    default:
                        map[i][k] = new Object { ' ' };
                }
            }
        }
    }
    

    
    bool isBlock(Object* obj) {
        if(dynamic_cast<Block*>(obj)) {
          return true;
        }
    
        return false;
    }
    bool isPlayer(Object* obj) {
        if(dynamic_cast<Player*>(obj)) {
            return true;
        }
        
        return false;
    }
    bool isGoal(Object* obj) {
        if(dynamic_cast<Goal*>(obj)) {
            return true;
        }
        
        return false;
    }
    

    void Show() const {
        using namespace std;
        
        for(int i=0; i<60; ++i) {
            cout << endl;
        }
        
        for(int i=0; i<height; ++i) {
            for(int k=0; k<width; ++k) {
                if(map[i][k] != nullptr) {
                    cout << map[i][k]->Simbol() << ' ';
                }
                else {
                    cout << ' ';
                }
            }
            cout << endl;
        }
        
        //for(int i=0; i<10; ++i) {
        //    cout << endl;
        //}
    }
    
    
    ObjectCode PlayerGoDown() {
        if(isGoal(map[player.position.y+1][player.position.x])) {
            return ObjectCode::Goal;
        }
        if(isBlock(map[player.position.y+1][player.position.x])) {
            return ObjectCode::Block;
        }
        
        player.position.y++;
        map[player.position.y][player.position.x] = new Player;
        delete map[player.position.y-1][player.position.x];
        map[player.position.y-1][player.position.x] = new Object { '.' };
        
        return ObjectCode::Empty;
    }
    ObjectCode PlayerGoUp() {
        if(isGoal(map[player.position.y-1][player.position.x])) {
            return ObjectCode::Goal;
        }
        if(isBlock(map[player.position.y-1][player.position.x])) {
            return ObjectCode::Block;
        }
        
        player.position.y--;
        map[player.position.y][player.position.x] = new Player;
        delete map[player.position.y+1][player.position.x];
        map[player.position.y+1][player.position.x] = new Object { '.' };
        
        return ObjectCode::Empty;
    }
    ObjectCode PlayerGoLeft() {
        if(isGoal(map[player.position.y][player.position.x-1])) {
            return ObjectCode::Goal;
        }
        if(isBlock(map[player.position.y][player.position.x-1])) {
            return ObjectCode::Block;
        }
        
        player.position.x--;
        map[player.position.y][player.position.x] = new Player;
        delete map[player.position.y][player.position.x+1];
        map[player.position.y][player.position.x+1] = new Object { '.' };
        
        return ObjectCode::Empty;
    }
    ObjectCode PlayerGoRight() {
        if(isGoal(map[player.position.y][player.position.x+1])) {
            return ObjectCode::Goal;
        }
        if(isBlock(map[player.position.y][player.position.x+1])) {
            return ObjectCode::Block;
        }
        
        player.position.x++;
        map[player.position.y][player.position.x] = new Player;
        delete map[player.position.y][player.position.x-1];
        map[player.position.y][player.position.x-1] = new Object { '.' };
            
        return ObjectCode::Empty;
    }
    bool PlayerGo(Player_::Direction dir) {
        switch(dir) {
            case Player_::Direction::Down:
                if(PlayerGoDown() == ObjectCode::Empty) {
                    return true;
                }
                break;
            case Player_::Direction::Up:
                if(PlayerGoUp() == ObjectCode::Empty) {
                    return true;
                }
                break;
            case Player_::Direction::Left:
                if(PlayerGoLeft() == ObjectCode::Empty) {
                    return true;
                }
                break;
            case Player_::Direction::Right:
                if(PlayerGoRight() == ObjectCode::Empty) {
                    return true;
                }
                break;
        }
        
        return false;
    }
    
    
    bool FindingRoad() {
        
    }
    
    

    ObjectCode Next() {
        switch(player.direc) {
            case Player_::Direction::Down:
                if(isBlock(map[player.position.y+1][player.position.x]))    return ObjectCode::Block;
                if(isGoal(map[player.position.y+1][player.position.x]))     return ObjectCode::Goal;
                /* else */                                                  return ObjectCode::Empty;
            case Player_::Direction::Up:
                if(isBlock(map[player.position.y-1][player.position.x]))    return ObjectCode::Block;
                if(isGoal(map[player.position.y-1][player.position.x]))     return ObjectCode::Goal;
                /* else */                                                  return ObjectCode::Empty;
            case Player_::Direction::Left:
                if(isBlock(map[player.position.y][player.position.x-1]))    return ObjectCode::Block;
                if(isGoal(map[player.position.y][player.position.x-1]))     return ObjectCode::Goal;
                /* else */                                                  return ObjectCode::Empty;
            case Player_::Direction::Right:
                if(isBlock(map[player.position.y][player.position.x+1]))    return ObjectCode::Block;
                if(isGoal(map[player.position.y][player.position.x+1]))     return ObjectCode::Goal;
                /* else */                                                  return ObjectCode::Empty;
        }
    }
    bool GoAhead() {
        return PlayerGo(player.direc);
    }
    void turnLeft() {
        player.direc = player.prevDirection();
    }
    void turnRight() {
        player.direc = player.nextDirection();
    }
    
    
    void rightHand() {
        //auto sleep_time = std::chrono::milliseconds(100);
        
        //Show();
        
        // 오른쪽 벽 짚고 가기
        
        // 벽에 붙기
        while(GoAhead()) {
            //std::this_thread::sleep_for(sleep_time);
            //Show();
        }
        
        // 골이냐
        if(Next() == ObjectCode::Goal) {
            std::cout << "Clear!" << std::endl;
            return;
        }
        
        // 아님 돌고
        //turnRight();
        turnLeft();
        
        // 탐색 시작
        while(1) {
            switch(Next()) {
                case ObjectCode::Empty:
                    GoAhead();
                    break;
                case ObjectCode::Goal:
                    Show();
                    std::cout << "Clear!" << std::endl;
                    return;
                case ObjectCode::Block:
                    //turnRight();
                    turnLeft();
                    continue;
            }
            
            //turnLeft();
            turnRight();
            
            //std::this_thread::sleep_for(sleep_time);
            //Show();
        }
    }
    
    void leftHand() {
        //auto sleep_time = std::chrono::milliseconds(100);
        
        //Show();
        
        // 왼쪽 벽 짚고 가기
        
        // 벽에 붙기
        while(GoAhead()) {
            //std::this_thread::sleep_for(sleep_time);
            //Show();
        }
        
        // 골이냐
        if(Next() == ObjectCode::Goal) {
            std::cout << "Clear!" << std::endl;
            return;
        }
        
        // 아님 돌고
        turnRight();
        
        // 탐색 시작
        while(1) {
            switch(Next()) {
                case ObjectCode::Empty:
                    GoAhead();
                    turnLeft();
                    break;
                case ObjectCode::Goal:
                    Show();
                    std::cout << "Clear!" << std::endl;
                    return;
                case ObjectCode::Block:
                    turnRight();
            }
            
            //std::this_thread::sleep_for(sleep_time);
            //Show();
        }
    }
    
    Player_::Direction randomDirection() {
        std::random_device rd;
        std::mt19937 gen { rd() };
        std::uniform_int_distribution<int> uid { 0, 3 };
        
        switch(uid(gen)) {
            case 0:
                return Player_::Direction::Up;
            case 1:
                return Player_::Direction::Right;
            case 2:
                return Player_::Direction::Down;
            case 3:
                return Player_::Direction::Left;
        }
    }
    
    void recursive_backtracking() {
        using namespace std;
        
        bool** visited = new bool*[height];
        for(int i=0; i<height; ++i) {
            visited[i] = new bool[width];
        }
        for(int i=0; i<height; ++i) {
            for(int k=0; k<width;++k) {
                if(isBlock(map[i][k])) {
                    visited[i][k] = true;
                }
            }
        }
        stack<Player_::Position> stk;
        
        bool repeat = true;
        while(repeat) {
            // 방문했다고 체크
            if(visited[player.position.y][player.position.x] == false) {
                //cout << "visit check" << endl;
                visited[player.position.y][player.position.x] = true;
                stk.push(player.position);
            }
            
            // 갇혔는지
            bool istrapped[4] = { false, false, false, false };
            
            // 왼쪽 ㄱㄴ?
            if(player.position.x > 0) {
                if(visited[player.position.y][player.position.x-1]) {
                    istrapped[0] = true;
                }
                else {
                    istrapped[0] = false;
                }
            }
            else {
                istrapped[0] = true;
            }
            // 오른쪽 ㄱㄴ?
            if(player.position.x < width-1) {
                if(visited[player.position.y][player.position.x+1]) {
                    istrapped[1] = true;
                }
                else {
                    istrapped[1] = false;
                }
            }
            else {
                istrapped[1] = true;
            }
            // 위 ㄱㄴ?
            if(player.position.y > 0) {
                if(visited[player.position.y-1][player.position.x]) {
                    istrapped[2] = true;
                }
                else {
                    istrapped[2] = false;
                }
            }
            else {
                istrapped[2] = true;
            }
            // 아래 ㄱㄴ?
            if(player.position.y < height-1) {
                if(visited[player.position.y+1][player.position.x]) {
                    istrapped[3] = true;
                }
                else {
                    istrapped[3] = false;
                }
            }
            else {
                istrapped[3] = true;
            }
            
            // 갇힘
            if(istrapped[0]) {
                if(istrapped[1]) {
                    if(istrapped[2]) {
                        if(istrapped[3]) {
                            // 이전셀로 이동
                            if(stk.empty()) {
                                repeat = false;
                                std::cout << "failed.." << std::endl;
                                break;
                            }
                            //std::cout << "back" << std::endl;
                            map[player.position.y][player.position.x] = new Object { '.' };
                            player.position = stk.top();
                            stk.pop();
                            continue;
                        }
                    }
                }
            }
                
            
            // 랜덤한 방향으로 이동
            switch(randomDirection()) {
                case Player_::Direction::Up:
                    player.direc = Player_::Direction::Up;
                    break;
                case Player_::Direction::Right:
                    player.direc = Player_::Direction::Right;
                    break;
                case Player_::Direction::Down:
                    player.direc = Player_::Direction::Down;
                    break;
                case Player_::Direction::Left:
                    player.direc = Player_::Direction::Left;
                    break;
            }
            
            if(Next() == ObjectCode::Goal) {
                Show();
                cout << "Clear!" << endl;
                repeat = false;
                break;
            }
            
            switch(player.direc) {
                case Player_::Direction::Down:
                    if(visited[player.position.y+1][player.position.x] == false) {
                        GoAhead();
                    }
                    break;
                case Player_::Direction::Up:
                    if(visited[player.position.y-1][player.position.x] == false) {
                        GoAhead();
                    }
                    break;
                case Player_::Direction::Left:
                    if(visited[player.position.y][player.position.x-1] == false) {
                        GoAhead();
                    }
                    break;
                case Player_::Direction::Right:
                    if(visited[player.position.y][player.position.x+1] == false) {
                        GoAhead();
                    }
                    break;
            }
        }
        
        for(int i=0; i<height; ++i) {
            delete[] visited[i];
        }
        delete[] visited;
    }
    
    
  public:
    Map(std::string* strmap, size_t width, size_t height) : width { width }, height { height }, map { nullptr }, player { } {
        map = new Object**[height];
        for(int i=0; i<height; ++i) {
            map[i] = new Object*[width];
            for(int k=0; k<width; ++k) {
                map[i][k] = nullptr;
            }
        }
        
        if(strmap == nullptr) {
            Initialize();
        }
        else {
            Initialize(strmap);
        }
    }

    
    void Play() {
        Show();
        
        // recursive_backtracking();
        leftHand();
    }
};



int main() {
    using namespace std;
    
    cout << "  [ Escape ]  " << endl << endl << endl << endl;
    /*
    string strmap[7] = {
        "######################################",
        "#P     #                #   #       G#",
        "#      #  ########## #  #      #######",
        "####  ##  #    #   ###  #####  #     #",
        "#     #        ##  #        #  ####  #",
        "#         #        #   #             #",
        "######################################"
    };*/
    // answer : 
    // "######################################",
    // "#P...  #............... #   #.......G#",
    // "#   .  #. ########## #. #    . #######",
    // "####. ##. #    #   ###. #####. #     #",
    // "#   . #..      ##  #  ......#. ####  #",
    // "#   ....  #        #   #   ...       #",
    // "######################################"
    
    //Map map { strmap, 38, 7 };
    //map.Play();
    
    Maze maze { 10, 12 };
        
    char** strmap = new char*[maze.height*3];
    for(int i=0; i<maze.height*3; ++i) {
        strmap[i] = new char[maze.width*3+1];
    }
    maze.map(strmap);
    
    string* stmp = new string[maze.height*3];
    for(int i=0; i<maze.height*3; ++i) {
        stmp[i] = string(strmap[i]);
    }
    Map map { stmp, maze.width*3, maze.height*3 };
    
    map.Play();
    
    delete[] stmp;
    for(int i=0; i<maze.height*3; ++i) {
        delete[] strmap[i];
    }
    delete[] strmap;
}


// 최적화 안됌
// 최단경로 아님
// 함수 정리 안됌
// 아 몰라
