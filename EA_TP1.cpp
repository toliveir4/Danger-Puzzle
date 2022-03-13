#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <algorithm>
#include <iterator>

using namespace std;

class Piece
{
public:
    vector<int> num;
    int row;
    int col;
    bool used;

    Piece() {}

    Piece(int n0, int n1, int n2, int n3)
    {
        num.push_back(n0);
        num.push_back(n1);
        num.push_back(n2);
        num.push_back(n3);
        row = 0;
        col = 0;
        used = false;
    }

    void Rotate(int value)
    {
        /* To get the starting point of rotated array */
        int mod = value % 4;

        vector<int> temp;

        // Prints the rotated array from start position
        for (int i = 0; i < 4; i++)
            temp.push_back(num[(mod + i) % 4]);

        num = temp;
    }

    pair<int, int> getBellowAbove()
    {
        return {num[2], num[1]};
    }

    pair<int, int> getLeftRight()
    {
        return {num[3], num[2]};
    }

    bool operator==(const Piece &rhs) const
    {
        return row == rhs.row && col == rhs.col && used == rhs.used && num == rhs.num;
    }
};

class Board
{
public:
    int n_pieces;
    int rows;
    int columns;
    vector<Piece *> pieces; // um vetor funciona como um Arraylist em Java, nao precisa de ter tamanho definido
    vector<Piece *> board;

    Board(int np, int r, int c)
    {
        n_pieces = np;
        rows = r;
        columns = c;
        pieces.resize(np); // define o tamanho do vetor de pecas
        // nao se faz o mesmo com o da board para ter metodo de comparacao
        // com o tamanho final do mesmo e o numero existente de pecas
    }

    void removeLeftPiece()
    {
        board.back()->used = false;
        board.back()->col = board.back()->row = 0;
        board.pop_back();
    }

    void removeToALimit(int index)
    {
        for (int i = board.size() - 1; i >= index; i--)
        {
            board[i]->used = false;
            board[i]->col = board[i]->row = 0;
            board.pop_back();
        }
    }

    void printBoard()
    {
        for (int row = 1; row <= this->rows; row++)
        {
            for (Piece *piece : this->board)
            {
                if (piece->row == row)
                {
                    cout << piece->num[0] << " " << piece->num[1];
                    if (piece->col != columns)
                        cout << "  ";
                    else
                        cout << endl;
                }
            }
            for (Piece *piece : this->board)
            {
                if (piece->row == row)
                {
                    cout << piece->num[3] << " " << piece->num[2];
                    if (piece->col != columns)
                        cout << "  ";
                    else
                        cout << endl;
                }
            }
            if (row != rows)
                cout << endl;
        }
    }
};

map<pair<int, int>, vector<Piece *>> pairs; // equivalente a dicionarios em python
map<int, int> occurrences;

void getPairsAndOccurrencies(Piece *p);
int checkMatch(Board *b, Piece *piece, int col, int row);
int checkMatchRight(Board *b, Piece *piece, int col, int row);
int checkMatchBellow(Board *b, int col, int row);
int checkMatchRightBellow(Board *b, Piece *piece, int col, int row);

bool tree(Board *b, int row, int col);

void reset(Board *b);

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;

    cin >> n;

    for (int _ = 0; _ < n; _++)
    {
        int np, r, c;

        cin >> np >> r >> c;
        Board *b = new Board(np, r, c);

        for (int i = 0; i < np; i++)
        {
            int n0, n1, n2, n3;
            cin >> n0 >> n1 >> n2 >> n3;
            Piece *p = new Piece(n0, n1, n2, n3);

            getPairsAndOccurrencies(p);

            if (i == 0)
            {
                p->col = p->row = 1;
                p->used = true;
                b->board.push_back(p);
            }

            b->pieces.push_back(p);
        }

        int odd = 0;
        for (auto &o : occurrences)
        {
            if (o.second % 2 != 0)
                odd++;
            if (odd > 4)
                break;
        }
        if (odd > 4)
        {
            cout << "impossible puzzle!\n";
            reset(b);
            continue;
        }

        /*// print do dicionario de pares
        for (auto &i : pairs)
        {
            cout << "(" << i.first.first << ", " << i.first.second << ")"
                 << " ";
            for (auto &j : i.second)
            {
                for (auto &k : j->num)
                    cout << k << " ";
                cout << ", ";
            }
            cout << endl;
        }*/
        /*
        bool res = tree(b, 1, 2);
        cout << res << endl;
        cout << "SIZE DA PUTA: " << b->board.size() << endl;
        */
        checkMatch(b, b->board[0], 1, 1);

        if ((int) b->board.size() == b->n_pieces)
            b->printBoard();
        else
            cout << "impossible puzzle!\n";
        reset(b);
    }
}

void getPairsAndOccurrencies(Piece *p)
{
    // percorre a peca e conta as occorencias de cada numero
    // cria ainda os 4 pares possiveis na peca
    for (int i = 0; i < 4; i++)
    {
        if (occurrences.find(p->num[i]) == occurrences.end())
            occurrences.insert({p->num[i], 1});
        else
            occurrences[p->num[i]]++;

        // o pair cria uma lista com 2 elementos que vai funcionar como key do map
        pair<int, int> pp = {p->num[i], p->num[(i + 1) % 4]};
        if (pairs.find(pp) == pairs.end())
            pairs[pp] = {p};
        else
            pairs[pp].push_back(p);
    }
}

int checkMatch(Board *b, Piece *piece, int col, int row)
{
    if (col == b->columns && row == b->rows)
        return 1;

    if (col < b->columns && row == 1)
    {
        checkMatchRight(b, piece, col, row);
    }

    if (row < b->rows && col == b->columns)
    {
        checkMatchBellow(b, col, row);
    }

    if (1 < row && row <= b->rows && 1 <= col && col <= b->columns)
    {
        checkMatchRightBellow(b, piece, col, row);
    }

    return 0;
}

int checkMatchRight(Board *b, Piece *piece, int col, int row)
{

    pair<int, int> p = piece->getBellowAbove();
    if (pairs.find(p) == pairs.end())
    {
        col--;
        b->removeLeftPiece();
        return 0;
    }

    for (auto &match : pairs[p])
    {
        // cout << " -> " << piece.num[0] << piece.num[1] << piece.num[2] << piece.num[3] << endl;
        // cout << match->operator==(piece) << endl;
        if (match->used == false)
        {
            int index = find(match->num.begin(), match->num.end(), p.second) - match->num.begin(); // indice do above
            int index2 = find(match->num.begin(), match->num.end(), p.first) - match->num.begin(); // indice do bellow

            if (index == (index2 + 1) % 4) // verifica se esta a usar o numero certo para a rotacao
                match->Rotate(index);
            else
            {
                match->Rotate((index2 + 1) % 4);
            }

            col++;
            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);

            checkMatch(b, match, col, row);
        }
    }

    return 0;
}

int checkMatchBellow(Board *b, int col, int row)
{
    int ind = (b->board.size() % b->columns) * row;

    Piece *piece = b->board[ind];
    pair<int, int> p = piece->getLeftRight();

    if (pairs.find(p) == pairs.end())   // se o par nao existir
    { 
        col = b->columns;
        row--;
        b->removeToALimit(ind);
        return 0;
    }

    for (auto &match : pairs[p])
    {
        if (match->used == false)
        {
            int index = find(match->num.begin(), match->num.end(), p.first) - match->num.begin(); // indice do left
            int index2 = find(match->num.begin(), match->num.end(), p.second) - match->num.begin(); // indice do right

            if (index == (index2 - 1) % 4) // verifica se esta a usar o numero certo para a rotacao
                match->Rotate(index);
            else
            {
                match->Rotate((index2 + 1) % 4);
            }
            row++;
            col = 1;
            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);
            
            checkMatch(b, match, col, row);
        }
    }

    return 0;
}

 vector<Piece *> Intersection(vector<Piece *>& nums1, vector<Piece *>& nums2) {
        
        sort(nums1.begin(), nums1.end()); // sort nums1 (non-decreasing order)
        sort(nums2.begin(), nums2.end()); // sort nums2 (non-decreasing order)
        
        int i = 0, j = 0;
        vector<Piece *> res;
        
        while(i < (int) nums1.size() && j < (int) nums2.size()){
            if(i > 0 && nums1[i - 1] == nums1[i]){
                i++; // dealing with duplicates in nums1
            }
            else if(j > 0 && nums2[j - 1] == nums2[j]){
                j++; // dealing with duplicates in nums2
            }
            else if(nums1[i] < nums2[j]){
                i++; // if nums1 < nums2, we increment nums1
            }
            else if(nums1[i] > nums2[j]){
                j++; // if nums1 > nums2, we increment nums2
            }
            else{
                res.push_back(nums1[i]); // if none of the above, nums1==nums2 so we add to result
                i++; j++;
            }
        }
        
        return res;
        
    }

int checkMatchRightBellow(Board *b, Piece *piece, int col, int row)
{
    pair<int, int> p = piece->getBellowAbove();

    if (pairs.find(p) == pairs.end())
    {
        col--;
        b->removeLeftPiece();
        return 0;
    }

    vector<Piece *> possibleVertical = pairs[p];

    int ind = (b->board.size() % b->columns) * (row - 1);

    Piece *piece2 = b->board[ind];
    pair<int, int> p2 = piece2->getLeftRight();

    if (pairs.find(p2) == pairs.end())   // se o par nao existir
    { 
        col = b->columns;
        row--;
        b->removeToALimit(ind);
        return 0;
    }


    vector<Piece *> possibleHorizontal = pairs[p2];

    vector<Piece *> intersection = Intersection(possibleHorizontal, possibleVertical);

    if (intersection.empty()) {
        col = b->columns;
        row--;
        b->removeToALimit(ind);
        return 0;
    }

    for (auto &match : intersection)
    {
        if (match->used == false)
        {
            int index = find(match->num.begin(), match->num.end(), p.second) - match->num.begin(); // indice do above
            int index2 = find(match->num.begin(), match->num.end(), p.first) - match->num.begin(); // indice do bellow

            if (index == (index2 + 1) % 4) // verifica se esta a usar o numero certo para a rotacao
                match->Rotate(index);
            else
            {
                match->Rotate((index2 + 1) % 4);
            }
            
            col++;
            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);
            
            checkMatch(b, match, col, row);
        }
    }


    return 0;
}

/*
bool tree(Board *b, int row, int col)
{
    int ind = (b->board.size() % b->columns) * (row - 1);

    Piece *piece = b->board[ind];

    pair<int, int> p = piece->getBellowAbove();
    if (pairs.find(p) == pairs.end()) // se o par nao existir
    {
        col--;
        b->removeLeftPiece();
        return false;
    }

    for (auto &match : pairs[p])
    {
        // cout << " -> " << piece.num[0] << piece.num[1] << piece.num[2] << piece.num[3] << endl;
        // cout << match->operator==(piece) << endl;
        if (match->used == false)
        {
            int index = find(match->num.begin(), match->num.end(), p.second) - match->num.begin(); // indice do above
            int index2 = find(match->num.begin(), match->num.end(), p.first) - match->num.begin(); // indice do bellow

            if (index == (index2 + 1) % 4) // verifica se esta a usar o numero certo para a rotacao
                match->Rotate(index);
            else
            {
                match->Rotate((index2 + 1) % 4);
            }

            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);
            // cout << "SIZE DA PUTA: " << b->board.size() << endl;
            if ((row * col) == b->n_pieces)
                return true;

            if (tree(b, row, col++) == true)
                return true;

            match->row = match->col = 0;
            match->used = false;
            int pieceIndex = find(b->board.begin(), b->board.end(), match) - b->board.begin();
            b->removeToALimit(pieceIndex);
        }
    }
}
*/

void reset(Board *b)
{
    b->board.clear();
    b->pieces.clear();
    pairs.clear();
    occurrences.clear();
    b->n_pieces = b->columns = b->rows = 0;
}