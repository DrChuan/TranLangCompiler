func int get(int[] mat, int i, int j, int n)
	int res := mat[i * n + j];
	return res;
endfunc

func int matrix_multiplication(int[] mat_a, int a_row, int a_col, int[] mat_b, int b_row, int b_col, int[] result)
	int i;
	int j;
	int k;
	int val;

	if (a_col != b_row)
		printString("Incompatible Dimensions\n");
		return 0;
	else
		i = 0;
		while (i < a_row)
			j = 0;
			while (j < b_col)
				k = 0;
				val = 0;
				while (k < a_col)
					val = val + mat_a[i * a_col + k] * mat_b[k * b_col + j];
					k = k + 1;
				endwhile
				result[i * b_col + j] = val;
				j = j + 1;
                k = 0;
			endwhile
			i = i + 1;
            j = 0;
		endwhile
	endif
	return 1;
endfunc


func int main()
	int i;
	int j;
	int a_row;
	int a_col;
	int b_row;
	int b_col;
    int ttt;
	int A[];
	int B[];
	int result[];
	string s := "\n";
	a_row = readInt();
	a_col = readInt();
	A = allocate(a_row * a_col);
	i = 0;
	j = 0;
	while (i < a_row)
		while (j < a_col)
            ttt = readInt();
			A[i * a_col + j] = ttt;
			ttt = get(A, i, j, a_row);
			j = j + 1;
		endwhile
		i = i + 1;
        j = 0;
	endwhile
	b_row = readInt();
	b_col = readInt();
	B = allocate(b_row * b_col);
	result = allocate(a_row * b_col);
	i = 0;
	j = 0;
	while (i < b_row)
		while (j < b_col)
			ttt = readInt();
			B[i * b_col + j] = ttt;
			j = j + 1;
		endwhile
		i = i + 1;
        j = 0;
	endwhile
	if (matrix_multiplication(A, a_row, a_col, B, b_row, b_col, result))
		i = 0;
		j = 0;
		while (i < a_row)
			while(j < b_col)
				printInt(result[i * b_col + j], 10);
				j = j + 1;
			endwhile
			printString(s);
			i = i + 1;
            j = 0;
		endwhile
	endif
	return 0;
endfunc







