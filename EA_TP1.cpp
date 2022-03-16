#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>

using namespace std;

class Piece
{
public:
    vector<int> num;
    int row;
    int col;
    bool used;
    bool tested;

    Piece(int n0, int n1, int n2, int n3)
    {
        num.push_back(n0);
        num.push_back(n1);
        num.push_back(n2);
        num.push_back(n3);

        row = 0;
        col = 0;
        used = false;
        tested = false;
    }

    void Rotate90()
    {
        int aux = num[1];
        num[1] = num[0];
        num[0] = num[3];
        num[3] = num[2];
        num[2] = aux;
    }

    void Rotate180()
    {
        int aux = num[2];
        num[2] = num[0];
        num[0] = aux;
        aux = num[3];
        num[3] = num[1];
        num[1] = aux;
    }

    void Rotate270()
    {
        int aux = num[3];
        num[3] = num[0];
        num[0] = num[1];
        num[1] = num[2];
        num[2] = aux;
    }

    pair<int, int> getBellowAbove()
    {
        return {num[2], num[1]};
    }

    pair<int, int> getLeftRight()
    {
        return {num[3], num[2]};
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

bool tree(Board *b, int row, int col);

bool addPrimeiraLinha(Board *b, int row, int col, pair<int, int> p);

bool addPrimeiraColuna(Board *b, int row, int col, pair<int, int> p);

bool addPeca(Board *b, int row, int col, vector<Piece *> intersection, vector<int> trio);

vector<Piece *> Intersection(vector<Piece *> &nums1, vector<int> trio);

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

    if (b == NULL || b->board.empty())
        return false; // verifica se o ponteiro para a Board existe ou nao e verifica se o vetor de pecas esta vazio

    // peca esta na primeira linha
    if (row == 1 && col <= b->columns)
    {
        piece = b->board[col - 2];

        // nao foi encontrada nenhuma peca com a coluna e a linha pretendida
        if (piece == NULL)
            return false;

        pair<int, int> p = piece->getBellowAbove();
        // se o par da peca da esquerda nao existir
        if (pairs.find(p) == pairs.end())
            return false;

        if (addPrimeiraLinha(b, 1, col, p))
            return true;
    }
    else if (row > 1 && row <= b->rows && col == 1)
    {
        // get peca de cima
        piece2 = b->board[(row - 2) * (b->columns)];

        // nao foi encontrada nenhuma peca de cima para as linhas depois da primeira
        if (piece2 == NULL)
            return false;

        pair<int, int> p2 = piece2->getLeftRight();

        //  se o par da peca de cima nao existir
        if (pairs.find(p2) == pairs.end())
            return false;

        if (addPrimeiraColuna(b, row, 1, p2)) // nao esta a aplicar a rotacao correta
            return true;

    }
    else if (row > 1 && row <= b->rows && col > 1 && col <= b->columns)
    {
        // get peca de cima e da esquerda
        piece = b->board[(row - 1) * b->columns + col - 2];      // peca da esquerda
        piece2 = b->board[(row - 2) * (b->columns) + (col - 1)]; // peca de cima

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

        // {bellow, above=left, right}
        vector<int> trio = {p.first, p.second, p2.second};

        // intersecao dos 2 vectors
        vector<Piece *> intersection = Intersection(possibleLeft, trio);

        if (intersection.empty())
            return false;

        /*for (auto &i : intersection)
        {
            cout << i->num[0] << " " << i->num[1] << " " << i->num[2] << " " << i->num[3] << endl;
        }
        cout << endl;*/

        if (addPeca(b, row, col, intersection, trio)) // nao esta a aplicar a rotacao correta
            return true;

    }
    return false;
}

bool addPrimeiraLinha(Board *b, int row, int col, pair<int, int> p)
{
    for (auto &match : pairs[p])
    {
        if (!match->used)
        {

            if (match->num[3] == p.second && match->num[2] == p.first)
                match->Rotate90();

            if (match->num[2] == p.second && match->num[1] == p.first)
                match->Rotate180();

            if (match->num[1] == p.second && match->num[0] == p.first)
                match->Rotate270();

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

            if (tree(b, row, col))
                return true;

            if (col == 1)
            { // se estiver no inicio da linha volta para o final da linha anterior
                row--;
                col = b->columns;
            }
            else
                col--;

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
        if (!match->used)
        {

            if (match->num[0] == p.second && match->num[3] == p.first)
                match->Rotate90();

            if (match->num[3] == p.second && match->num[2] == p.first)
                match->Rotate180();

            if (match->num[2] == p.second && match->num[1] == p.first)
                match->Rotate270();

            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);

            if ((row * col) == b->n_pieces)
                return true;

            if (b->columns == 1)
                row++;
            else
            {
                col++;
            }

            if (tree(b, row, col))
                return true;

            if (b->columns == 1)
                row--;
            else
                col--;

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
        if (!match->used)
        {

            if (match->num[0] == trio[2] && match->num[3] == trio[1] && match->num[2] == trio[0])
                match->Rotate90();

            if (match->num[3] == trio[2] && match->num[2] == trio[1] && match->num[1] == trio[0])
                match->Rotate180();

            if (match->num[2] == trio[2] && match->num[1] == trio[1] && match->num[0] == trio[0])
                match->Rotate270();

            match->col = col;
            match->row = row;
            match->used = true;

            b->board.push_back(match);

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

            if (tree(b, row, col))
                return true;

            // se estiver na ultima coluna volta para a linha de cima
            if (col == 1)
            {
                row--;
                col = b->columns;
            }
            else
                col--;

            int pieceIndex = find(b->board.begin(), b->board.end(), match) - b->board.begin();
            b->removeToALimit(pieceIndex);
        }
    }
    return false;
}

vector<Piece *> Intersection(vector<Piece *> &nums1, vector<int> trio)
{

    sort(nums1.begin(), nums1.end()); // sort nums1 (non-decreasing order)

    vector<Piece *> res;

    for (auto &n : nums1)
    {
        for (int j = 0; j < 4; j++)
        {
            if (n->num[j] == trio[0] && n->num[(j + 1) % 4] == trio[1] && n->num[(j + 2) % 4] == trio[2])
            {
                res.push_back(n);
                break;
            }
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

    int n = 0;

    cin >> n;

    for (int _ = 0; _ < n; _++)
    {
        int np = 0, r = 0, c = 0;

        cin >> np >> r >> c;
        Board *b = new Board(np, r, c);

        for (int i = 0; i < np; i++)
        {
            int n0, n1, n2, n3 = 0;
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

        bool res;
        if (b->columns == 1)
            res = tree(b, 2, 1);
        else
            res = tree(b, 1, 2);

        if (res)
            b->printBoard();
        else
            cout << "impossible puzzle!\n";
        reset(b);
    }
}