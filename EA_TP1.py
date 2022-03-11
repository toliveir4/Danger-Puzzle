from sys import stdin, stdout


class Pieces:
    numbers = []
    col = 0
    row = 0
    used = False

    def getPairsAndOccurrencies(self) -> None:
        for pi in range(len(self.numbers)):
            if b.occurencies.get(self.numbers[pi]) is None:
                b.occurencies[self.numbers[pi]] = 1
            else:
                b.occurencies[self.numbers[pi]] += 1

            if b.pairs.get((self.numbers[pi], self.numbers[(pi + 1) % 4])) is None:
                b.pairs[(self.numbers[pi], self.numbers[(pi + 1) % 4])] = [self]
            else:
                b.pairs[(self.numbers[pi], self.numbers[(pi + 1) % 4])].append(self)


class Board:
    n_pieces = rows = columns = 0
    pieces_arr = []
    board = []
    pairs = {}
    occurencies = {}

    def remove_piece(self, ind):
        self.board[ind].used = False
        self.board[ind].col = 0
        self.board[ind].row = 0
        self.board.pop(ind)

    def remove_pieces(self, ind):
        for i in range(len(self.board) - 1, ind - 1, -1):
            self.board[i].used = False
            self.board[i].row = 0
            self.board[i].col = 0
            self.board.pop(i)

    def printBoard(self):
        for r in range(self.rows):
            for piece in self.board:
                if piece.row == r + 1:
                    outln(piece.numbers[0])
                    outln(" ")
                    outln(piece.numbers[1])
                    if piece.col != self.columns:
                        outln("  ")
                    else:
                        outln("\n")

            for piece in self.board:
                if piece.row == r + 1:
                    outln(piece.numbers[3])
                    outln(" ")
                    outln(piece.numbers[2])
                    if piece.col != self.columns:
                        outln("  ")
                    else:
                        outln("\n")
            if r + 1 != self.rows:
                outln("\n")


def checkMatch(piece1: Pieces, col: int, row: int) -> int:
    if col == b.columns and row == b.rows:
        return 1

    if col < b.columns and row == 1:
        r = checkMatchRight(piece1, col, row)
        """if r == 0:
            return 0"""

    if row < b.rows and col == b.columns:
        r = checkMatchBellow(col, row)
        """if r == 0:
            return 0"""

    if 1 < row <= b.rows and 1 <= col <= b.columns:
        r = checkMatchRightBellow(piece1, col, row)
        """if r == 0:
            return 0"""

    return 0


def checkMatchRight(piece1: Pieces, col: int, row: int) -> int:
    above = piece1.numbers[1]
    bellow = piece1.numbers[2]

    if b.pairs.get((bellow, above)) is None:
        col -= 1
        b.remove_piece(b.board.index(piece1))
        return 0

    for match in b.pairs[(bellow, above)]:
        if match.used is False:
            match.numbers = match.numbers[match.numbers.index(above):] + \
                            match.numbers[:match.numbers.index(above)]

            col += 1
            match.col = col
            match.row = row
            match.used = True
            b.board.append(match)

            ret = checkMatch(match, col, row)
            if ret == 1:
                return 1
            """else:
                print("Continue right")
                col -= 1
                b.remove_piece(b.board.index(match))
                continue"""
    return 0


def checkMatchBellow(col: int, row: int) -> int:
    ind = (len(b.board) % b.columns) * row
    print("row", row, "col", col)
    print("indice", ind)
    print(len(b.board))
    print("-"*20)
    right = b.board[ind].numbers[2]
    left = b.board[ind].numbers[3]

    if b.pairs.get((left, right)) is None:
        row -= 1
        col = b.columns
        b.remove_pieces(ind)
        return 0
    for match in b.pairs[(left, right)]:
        if match.used is False:
            match.numbers = match.numbers[match.numbers.index(left):] + \
                            match.numbers[:match.numbers.index(left)]

            row += 1
            col = 1
            match.col = col
            match.row = row
            match.used = True
            b.board.append(match)

            ret = checkMatch(match, col, row)
            if ret == 1:
                return 1
            """else:
                print("Continue bellow")
                row -= 1
                col = b.columns
                b.remove_piece(b.board.index(match))
                continue"""
    return 0


def checkMatchRightBellow(piece1: Pieces, col: int, row: int) -> int:
    above = piece1.numbers[1]
    bellow = piece1.numbers[2]
    print(piece1.numbers)
    possible_vertical = b.pairs.get((bellow, above))

    if possible_vertical is None:
        col -= 1
        b.remove_piece(b.board.index(piece1))
        return 0

    ind = (len(b.board) % b.columns) * (row - 1)
    print("row", row-1, "col", col+1)
    print("indice", ind)
    print(len(b.board))
    print(b.board[ind].numbers)
    print()
    right = b.board[ind].numbers[2]
    left = b.board[ind].numbers[3]

    possible_horizontal = b.pairs.get((left, right))

    if possible_horizontal is None:
        col -= 1
        b.remove_piece(b.board.index(piece1))
        return 0

    intersection = list(set(possible_horizontal) & set(possible_vertical))
    if intersection is []:
        col -= 1
        b.remove_piece(b.board.index(piece1))
        return 0
    print(f"bellow {bellow} - above {above} - left {left} - right {right}")

    for match in intersection:
        if match.used is False:

            print("piece", p.numbers)
            print("*"*20)
            match.numbers = match.numbers[match.numbers.index(above):] + \
                            match.numbers[:match.numbers.index(above)]
            col += 1
            match.col = col
            match.row = row
            match.used = True
            b.board.append(match)

            ret = checkMatch(match, col, row)
            if ret == 1:
                return 1
            """else:
                print("Continue right bellow")
                col -= 1
                b.remove_piece(b.board.index(match))
                continue"""
    return 0


def reset():
    global p
    p = Pieces()
    b.remove_pieces(0)
    for i in range(b.n_pieces - 1, -1, -1):
        b.pieces_arr.pop(i)
    b.pairs = {}
    b.occurencies = {}
    b.rows = b.columns = b.n_pieces = 0


def readln():
    return stdin.readline().rstrip()


def outln(s1):
    stdout.write(str(s1))


if __name__ == "__main__":
    n = int(input())
    b = Board()

    for _ in range(n):
        b.n_pieces, b.rows, b.columns = map(int, input().split())

        for i in range(b.n_pieces):
            p = Pieces()
            p.numbers = list(map(int, input().split()))
            b.pieces_arr.append(p)

            p.getPairsAndOccurrencies()

            if i == 0:
                p.used = True
                p.row = 1
                p.col = 1
                b.board.append(p)

        odd = 0
        for o in b.occurencies:
            if b.occurencies[o] % 2 != 0:
                odd += 1
            if odd > 4:
                break
        if odd > 4:
            outln("impossible puzzle!\n")
            reset()
            continue

        r = checkMatch(b.board[0], 1, 1)

        if len(b.board) == b.n_pieces:
            b.printBoard()
        else:
            outln("impossible puzzle!\n")
        reset()
