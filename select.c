#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randomized_partition(int a[], int p, int r){

	//get random value
	int i = p + rand()%(r-p+1), j;
	int temp;
	int pivot;

	//swap
	temp = a[r];
	a[r] = a[i];
	a[i] = temp;

	//partition
	pivot = a[r];
	i = p-1;
	for(j=p;j<r;j++){
		if(a[j] <= pivot){
			i++;
			temp = a[j];
			a[j] = a[i];
			a[i] = temp;
		}
	}
	a[r] = a[i+1];
	a[i+1] = pivot;	
	return i+1;
}

int randomized_select_rev(int a[], int p, int r, int k) {

	int q;
	int i;

	if(p==r)
		return a[p];
	
	q = randomized_partition(a, p, r);	
	i = q-p;

	if(k == i)
		return a[q];
	else if(k < i)
		return randomized_select_rev(a, p, q-1, k);
	else
		return randomized_select_rev(a, q+1, r, k-i-1);
}

int randomized_select(int a[], int n, int k){

    int result;
//	clock_t begin = clock(), end;

    //randomize
    srand(time(NULL));
    result = randomized_select_rev(a, 0, n-1, k);
//	end = clock();
//	printf("%lf\n", (double)(end - begin)/CLOCKS_PER_SEC);

    return result;
}

int partition_rev(int a[], int n, int median){

	int i, j;
	int temp;
	int pivot;

	for(i=0;i<n;i++){
		if(a[i] == median)
			break;
	}

	//swap
	temp = a[n-1];
	a[n-1] = a[i];
	a[i] = temp;

	//partition
	pivot = a[n-1];
	i = -1;
	for(j=0;j<n-1;j++){
		if(a[j] <= pivot){
			i++;
			temp = a[j];
			a[j] = a[i];
			a[i] = temp;
		}
	}
	a[n-1] = a[i+1];
	a[i+1] = pivot;	

	return i+1;
}

void insert_sort(int **temp, int n){

	int i, j, k;
	int num_groups = (n-1)/5 + 1;
	int num = 5;
	int key;
	
	for(k=0;k<num_groups;k++){
		if(k==num_groups-1)
			num = n-5*k;
		for(j=1;j<num;j++){
			key = temp[k][j];
			i = j - 1;
			while(i>=0 && temp[k][i]>key){
				temp[k][i+1] = temp[k][i];
				i--;
			}
			temp[k][i+1] = key;
		}
	}
}

int find_median(int **temp, int n){

	int i;
	int num_groups = (n-1)/5 + 1;
	int remain_num = n-5*(num_groups-1);

	if(n == 1)
		return temp[0][0];
	else if(n == 2)
		return ((temp[0][0] < temp[0][1]) ? temp[0][0] : temp[0][1]);

	insert_sort(temp, n);

	//set medians only
	for(i=0;i<num_groups;i++){
		if(i<num_groups-1)
			temp[i/5][i%5] = temp[i][2];
		else
			temp[i/5][i%5] = temp[i][remain_num/2];
	}
	return find_median(temp, num_groups);
}

int deterministic_select_rev(int a[], int start, int n, int k, int **temp){

	int i;
	int median;
	int pos;

	for(i=0;i<n;i++){
		temp[i/5][i%5] = a[start+i];
	}

	median = find_median(temp, n);
	
	pos = partition_rev(a+start, n, median);

	if(pos == k)
		return median;
	else if(pos > k)
		return deterministic_select_rev(a, start, pos, k, temp);
	else{
		start += (pos + 1);
		return deterministic_select_rev(a, start, n-1-pos, k-pos-1, temp);
	}
}

int deterministic_select(int a[], int n, int k){

	int i;
	int *temp[400000];
	int result;
	clock_t begin, end;

	for(i=0;i<(n-1)/5+1;i++)
		temp[i] = (int*)malloc(5*sizeof(int));

//	begin = clock();
	result = deterministic_select_rev(a, 0, n, k, temp);
//	end = clock();
//	printf("%lf\n", (double)(end - begin)/CLOCKS_PER_SEC);	

	for(i=0;i<(n-1)/5+1;i++)
		free(temp[i]);

	return result;
}

int checker(int a[], int n, int k, int ans){

    int i;
    int smaller = 0;
    int ifExists = 0;

    for(i=0;i<n;i++){
		if(a[i] < ans)
	    	smaller++;
		else if (a[i] == ans)
	    	ifExists = 1;
    }
    if(ifExists && smaller==k)
		return 1;
	else
    	return 0;
}
