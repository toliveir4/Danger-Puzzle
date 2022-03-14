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
        vector<int> temp;

        // Prints the rotated array from start position
        for (int i = 0; i < 4; i++)
            temp.push_back(num[(value + i) % 4]);

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

/*
int checkMatch(Board *b, Piece *piece, int col, int row);
int checkMatchRight(Board *b, Piece *piece, int col, int row);
int checkMatchBellow(Board *b, int col, int row);
int checkMatchRightBellow(Board *b, Piece *piece, int col, int row);
*/

void getPairsAndOccurrencies(Piece *p);
bool tree(Board *b, int row, int col);
bool addPrimeiraLinha(Board *b, int row, int col, pair<int, int> p);
bool addPrimeiraColuna(Board *b, int row, int col, pair<int, int> p);
bool addPeca(Board *b, int row, int col, vector<Piece *> intersection, vector<int> trio);
vector<Piece *> Intersection(vector<Piece *> &nums1, vector<Piece *> &nums2);
void reset(Board *b);
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

bool tree(Board *b, int row, int col)
{
    Piece *piece = NULL;
    Piece *piece2 = NULL;

    cout << "row: " << row << " col: " << col << endl;

    if (b == NULL || b->board.empty())
        return false; // verifica se o ponteiro para a Board existe ou nao e verifica se o vetor de pecas esta vazio

    // peca esta na primeira linha
    if (row == 1)
    {
        piece = b->board[col - 2];

        // nao foi encontrada nenhuma peca com a coluna e a linha pretendida
        if (piece == NULL)
            return false;

        pair<int, int> p = piece->getBellowAbove();
        // se o par da peca da esquerda nao existir
        if (pairs.find(p) == pairs.end())
            return false;

        if (addPrimeiraLinha(b, row, col, p))
        {
            return true;
        }
    }
    else if (row > 1 && col == 1)
    {
        // get peca de cima
        piece2 = b->board[(row - 2) * (b->columns)];

        // cout << piece2->num[0] << " " << piece2->num[1] << " " << piece2->num[2] << " " << piece2->num[3] << endl;

        // nao foi encontrada nenhuma peca de cima para as linhas depois da primeira
        if (piece2 == NULL)
            return false;

        pair<int, int> p2 = piece2->getLeftRight();

        // cout << "PAR: " << p2.first << " " << p2.second << endl;
        //  se o par da peca de cima nao existir
        if (pairs.find(p2) == pairs.end())
            return false;

        if (addPrimeiraColuna(b, row, col, p2))
        { // nao esta a aplicar a rotacao correta
            return true;
        }
    }
    else if (row > 1 && col > 1)
    {
        // get peca de cima e da esquerda
        piece = b->board[(row - 1) * b->columns + col - 2]; // peca da esquerda
        piece2 = b->board[(row - 2)*(b->columns) + (col - 1)]; // peca de cima

        cout << "\nteste\n";
        cout << piece->num[0] << " " << piece->num[1] << " " << piece->num[2] << " " << piece->num[3] << endl;
        cout << piece2->num[0] << " " << piece2->num[1] << " " << piece2->num[2] << " " << piece2->num[3] << endl;
        cout << "end\n\n";
        // nao foi encontrada nenhuma peca com a coluna e a linha pretendida
        // nao foi encontrada nenhuma peca de cima para as linhas depois da primeira
        if (piece == NULL || piece2 == NULL)
            return false;

        pair<int, int> p = piece->getBellowAbove();
        // se o par da peca da esquerda nao existir
        if (pairs.find(p) == pairs.end())
            return false;

        pair<int, int> p2 = piece2->getLeftRight();
        // se o par da peca de cima nao existir
        if (pairs.find(p2) == pairs.end())
            return false;

        // vector com pecas que encaixam segundo o par da peca da esquerda
        vector<Piece *> possibleLeft = pairs[p];

        // vector com pecas que encaixam segundo o par da peca de cima
        vector<Piece *> possibleBellow = pairs[p2];

        // intersecao dos 2 vectors
        vector<Piece *> intersection = Intersection(possibleBellow, possibleLeft);

        if (intersection.empty())
            return false;

        // {bellow, above=left, right}
        vector<int> trio = {p.first, p.second, p2.second};

        if (addPeca(b, row, col, intersection, trio))
        { // nao esta a aplicar a rotacao correta
            return true;
        }
    }
    return false;
}

bool addPrimeiraLinha(Board *b, int row, int col, pair<int, int> p)
{
    for (auto &match : pairs[p])
    {
        //  cout << match->operator==(piece) << endl;
        if (match->used == false)
        {
            // cout << match->num[0] << " " << match->num[1] << " " << match->num[2] << " " << match->num[3] << endl;

            int index = find(match->num.begin(), match->num.end(), p.second) - match->num.begin(); // indice do above
            int index2 = find(match->num.begin(), match->num.end(), p.first) - match->num.begin(); // indice do bellow

            if (index == (index2 + 1) % 4) // verifica se esta a usar o numero certo para a rotacao
                match->Rotate(index);
            else
                match->Rotate((index2 + 1) % 4);

            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);

            if ((row * col) == b->n_pieces)
                return true;

            if (col == b->columns) // se estiver no fim da linha passa para a de baixo
            {
                row++;
                col = 1;
            }
            else
                col++;

            cout << match->num[0] << " " << match->num[1] << " " << match->num[2] << " " << match->num[3] << endl;
            if (tree(b, row, col) == true)
                return true;

            if (col == 1)
            { // se estiver no inicio da linha volta para o final da linha anterior
                row--;
                col = b->columns;
            }
            else
                col--;

            match->row = match->col = 0;
            match->used = false;
            int pieceIndex = find(b->board.begin(), b->board.end(), match) - b->board.begin();
            b->removeToALimit(pieceIndex);
        }
    }
    return false;
}

bool addPrimeiraColuna(Board *b, int row, int col, pair<int, int> p)
{
    for (auto &match : pairs[p])
    {
        if (match->used == false)
        {
            int index = find(match->num.begin(), match->num.end(), p.second) - match->num.begin(); // indice do right
            int index2 = find(match->num.begin(), match->num.end(), p.first) - match->num.begin(); // indice do left

            // cout << index << " " << index2 << endl;

            if (index2 == (index - 1) % 4)
            { // verifica se esta a usar o numero certo para a rotacao
                match->Rotate(index2);
                // cout << "right\n";
                // cout << match->used << "\t\t" << " -> " << match->num[0] << match->num[1] << match->num[2] << match->num[3] << endl;
            }
            else
            {
                // cout << "\t\t" << " -> " << match->num[0] << match->num[1] << match->num[2] << match->num[3] << endl;
                match->Rotate((index + 1) % 4);
                // cout << "\t\t" << " -> " << match->num[0] << match->num[1] << match->num[2] << match->num[3] << endl;
            }

            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);

            if ((row * col) == b->n_pieces)
                return true;

            if (b->columns == 1)
                row++;
            else
                col++;

            cout << match->num[0] << " " << match->num[1] << " " << match->num[2] << " " << match->num[3] << endl;
            // cout << "row: " << row << endl;
            if (tree(b, row, col) == true)
                return true;

            // volta para o final da linha anterior
            row--;
            col = b->columns;

            match->row = match->col = 0;
            match->used = false;
            int pieceIndex = find(b->board.begin(), b->board.end(), match) - b->board.begin();
            b->removeToALimit(pieceIndex);
        }
    }

    return false;
}

bool addPeca(Board *b, int row, int col, vector<Piece *> intersection, vector<int> trio)
{
    for (auto &match : intersection)
    {
        cout << "\t\t" << " -> " << match->num[0] << match->num[1] << match->num[2] << match->num[3] << endl;
        if (match->used == false)
        {

            int index = find(match->num.begin(), match->num.end(), trio[0]) - match->num.begin();  // bellow
            int index2 = find(match->num.begin(), match->num.end(), trio[1]) - match->num.begin(); // above = left
            int index3 = find(match->num.begin(), match->num.end(), trio[2]) - match->num.begin(); // right

            
            if (index2 == (index - 1) % 4 && index2 == (index3 + 1) % 4)
            { // verifica se esta a usar o numero certo para a rotacao
                match->Rotate(index2);
                // cout << "right\n";
                // cout << match->used << "\t\t" << " -> " << match->num[0] << match->num[1] << match->num[2] << match->num[3] << endl;
            }
            else
            {
                // cout << "\t\t" << " -> " << match->num[0] << match->num[1] << match->num[2] << match->num[3] << endl;
                match->Rotate((index + 1) % 4);
                // cout << "\t\t" << " -> " << match->num[0] << match->num[1] << match->num[2] << match->num[3] << endl;
            }

            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);

            cout << "row*col" << row*col << endl;

            if ((row * col) == b->n_pieces)
                return true;

            // se estiver na ultima coluna passa para a linha de baixo
            if (col == b->columns)
            {
                col = 1;
                row++;
            }
            else
                col++;

            if (tree(b, row, col) == true)
                return true;

            // se estiver na ultima coluna volta para a linha de cima
            // if(col == 1) {
            //     row--;
            //     col = b->columns;
            // } else
            col--;

            match->row = match->col = 0;
            match->used = false;
            int pieceIndex = find(b->board.begin(), b->board.end(), match) - b->board.begin();
            b->removeToALimit(pieceIndex);
        }
    }

    return false;
}

vector<Piece *> Intersection(vector<Piece *> &nums1, vector<Piece *> &nums2)
{

    sort(nums1.begin(), nums1.end()); // sort nums1 (non-decreasing order)
    sort(nums2.begin(), nums2.end()); // sort nums2 (non-decreasing order)

    int i = 0, j = 0;
    vector<Piece *> res;

    while (i < (int)nums1.size() && j < (int)nums2.size())
    {
        if (i > 0 && nums1[i - 1] == nums1[i])
            i++; // dealing with duplicates in nums1

        else if (j > 0 && nums2[j - 1] == nums2[j])
            j++; // dealing with duplicates in nums2

        else if (nums1[i] < nums2[j])
            i++; // if nums1 < nums2, we increment nums1

        else if (nums1[i] > nums2[j])
            j++; // if nums1 > nums2, we increment nums2

        else
        {
            res.push_back(nums1[i]); // if none of the above, nums1==nums2 so we add to result
            i++;
            j++;
        }
    }

    return res;
}

void reset(Board *b)
{
    b->board.clear();
    b->pieces.clear();
    pairs.clear();
    occurrences.clear();
    b->n_pieces = b->columns = b->rows = 0;
}

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

        bool res = false;
        if (b->columns == 1)
            res = tree(b, 2, 1);
        else
            res = tree(b, 1, 2);
        // cout << "RESULT: " << res << endl;
        // cout << "SIZE: " << b->board.size() << endl;

        // checkMatch(b, b->board[0], 1, 1);

        if (res)
            b->printBoard();
        else
            cout << "impossible puzzle!\n";
        reset(b);
    }
}

/**********************************NAO ESTA A SER USADO**********************************/

/*
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
                match->Rotate((index2 + 1) % 4);

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

    if (pairs.find(p) == pairs.end()) // se o par nao existir
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
            int index = find(match->num.begin(), match->num.end(), p.first) - match->num.begin();   // indice do left
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

int checkMatchRightBellow(Board *b, Piece *piece, int col, int row)
{
    pair<int, int> p = piece->getBellowAbove();

    if (pairs.find(p) == pairs.end())
    {
        col--;
        b->removeLeftPiece();
        return 0;
    }

    vector<Piece *> possibleLeft = pairs[p];

    int ind = (b->board.size() % b->columns) * (row - 1);

    Piece *piece2 = b->board[ind];
    pair<int, int> p2 = piece2->getLeftRight();

    if (pairs.find(p2) == pairs.end()) // se o par nao existir
    {
        col = b->columns;
        row--;
        b->removeToALimit(ind);
        return 0;
    }

    vector<Piece *> possibleBellow = pairs[p2];

    vector<Piece *> intersection = Intersection(possibleBellow, possibleLeft);

    if (intersection.empty())
    {
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
*/