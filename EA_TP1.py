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

    def remove_pieces(self):
        for i in range(len(self.board) - 1, 0, -1):
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
        checkMatchRight(piece1, col, row)

    if row < b.rows:
        checkMatchBellow(col, row)

    if 1 < row <= b.rows and 1 <= col <= b.columns:
        checkMatchRightBellow(piece1, col, row)
    
    return 0


def checkMatchRight(piece1: Pieces, col: int, row: int) -> int:
    above = piece1.numbers[1]
    bellow = piece1.numbers[2]

    if b.pairs.get((bellow, above)) is None:
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
    return 0


def checkMatchBellow(col: int, row: int) -> int:
    for p in b.board:
        if p.row == row and p.col == 1:
            right = p.numbers[2]
            left = p.numbers[3]
            if b.pairs.get((left, right)) is None:
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
    return 0


def checkMatchRightBellow(piece1: Pieces, col: int, row: int) -> int:
    above = piece1.numbers[1]
    bellow = piece1.numbers[2]

    possible_horizontal = b.pairs[(bellow, above)]
    if possible_horizontal is None:
        return 0

    for p in b.board:
        if p.row == row - 1 and p.col == col + 1:
            right = p.numbers[2]
            left = p.numbers[3]

            possible_vertical = b.pairs[(left, right)]
            if possible_vertical is None:
                return 0
            intersection = possible_horizontal and possible_vertical
            if intersection is None:
                return 0

            for p in intersection:
                if p.used is False:
                    p.numbers = p.numbers[p.numbers.index(above):] + \
                                p.numbers[:p.numbers.index(above)]
                    col += 1
                    p.col = col
                    p.row = row
                    p.used = True
                    b.board.append(p)

                    ret = checkMatch(p, col, row)
                    if ret == 1:
                        return 1
    return 0


def reset():
    global p
    p = Pieces()
    b.remove_pieces()
    b.board.pop(0)
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
            outln("impossible puzzle!odd\n")
            reset()
            continue

        checkMatch(b.board[0], 1, 1)
        for i in b.board:
            print(i.numbers)

        if len(b.board) == b.n_pieces:
            b.printBoard()
        else:
            outln("impossible puzzle!\n")
        reset()
