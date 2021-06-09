func void quick_sort(int []array, int left, int right)
	if (left >= right)
		return;
	endif
	int base := array[left];
	int i := left;
    int j := right;
	while (i < j)
		while (array[j] >= base && i < j)
			j = j - 1;
		endwhile
		if (i < j)
			array[i] = array[j];
			i = i + 1;
		endif
		while (array[i] < base && i < j)
			i = i + 1;
		endwhile
		if (i < j)
			array[j] = array[i];
			j = j - 1;
		endif
	endwhile
	array[i] = base;
	quick_sort(array, left, i - 1);
	quick_sort(array, i + 1, right);
endfunc


func int main()
	int N;
	int i;

	N = readInt();
	int array[] := allocate(N);
	i = 0;
	while (i < N)
		array[i] = readInt();
		i = i + 1;
	endwhile
	quick_sort(array, 0, N - 1);
	i = 0;
	while (i < N)
		printInt(array[i], 0);
		printString("\n");
		i = i + 1;
	endwhile
	return 0;
endfunc