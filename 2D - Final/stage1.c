#include<stdio.h>
#define s 50
struct corr
{
    int x;
    int y;
}point[s],upper_hull[s],lower_hull[s],upper[s],lower[s],hull[s];


// find on which side of the mid line thus the point lie in order to put it in appropriate 1st half or 2nd half
double find_val(struct corr a, struct corr b, struct corr c)
{
        return (b.y - a.y)*c.x + (a.x - b.x)*c.y - (a.x*b.y - a.y*b.x);
}

double find_line (struct corr a, struct corr b, struct corr c)
{
	return ((b.x - a.x)*(c.y - a.y)-(b.y-a.y)*(c.x-a.x));
}


// display the formed hull
void display(const struct corr a[], const int len)
{
	int i;
        for(i=0;i<len;i++)
        {
                printf("\t(%d,%d)",a[i].x,a[i].y);
        }
	printf("\n");
}


// sort the points in 1.ascending order if x cordinate and then decending order of y cordinates
int sort(struct corr point[], int n)
{
	int i, j;
	int data, data1;
	for(i=0;i<n-1;i++) // sort operation
        {
                for(j=0;j<n-i-1;j++)
                {
                        if(point[j].x > point[j+1].x) // increasing x cordinate
                        {
                                data=point[j].y;
                                data1=point[j].x ;
                                point[j].y=point[j+1].y;
                                point[j].x=point[j+1].x;
                                point[j+1].y=data;
                                point[j+1].x=data1;
                        }
                        else if(point[j].x == point[j+1].x)
			{
				if(point[j].y < point[j+1].y) // if the x cordinates of two points are same, sorting wrt decreasing y cordinates
                                {
                                        data=point[j].y;
                                        data1=point[j].x ;
                                        point[j].y=point[j+1].y;
                                        point[j].x=point[j+1].x;
                                        point[j+1].y=data;
                                        point[j+1].x=data1;
                                }
			}
                  }
        }
	for(i = 1 ; i < n; ++i)
	{
		if((point[i-1].x == point[i].x) && (point[i-1].y == point[i].y))
		{
			for(j = i+1; j < n; ++j)
			{
				point[j-1].x = point[j].x;
				point[j-1].y = point[j].y;
			}
			n--;
		}
	}
	return n;
}

// calculating determinant in order to find whether the area of triangle is positive or negetive
double is_convex(struct corr a, struct corr b, struct corr c)
{
	double d1, d2, d3;
	d1 = a.x*b.y-a.x*c.y;
        d2 = a.y*b.x-a.y*c.x;
        d3 = b.x*c.y-c.x*b.y;
        return (d1+d3-d2);
}


// joining the upper and lower hull to get the entire structure
int merging(struct corr h[], struct corr l[], struct corr u[], int lp, int up)
{
	int i, j;
	for(i = 0; i < lp; ++i)
	{
		h[i].x = l[i].x;
		h[i].y = l[i].y;
	}
	for(j = up-2; j >= 0; --j, ++i)
	{
		h[i].x = u[j].x;
		h[i].y = u[j].y;
	}
	return i; // return the end point
}

int check_hull(const struct corr a[], const struct corr p[], int n, int m)
{
	int i, j, g, l;
	double val;
	for(i = 0; i < m-1; ++i)
	{
		g = 0; l = 0;
		for(j = 0 ; j < n ; ++j)
		{
			val = find_line(a[i], a[i+1], p[j]);
			if(val > 0)
				g = 1;
			else if(val < 0)
				l = 1;
		}
		if(g == 1 && l == 1)
			return 0;
	}
	return 1;
}

int main()
{
	struct corr left, right;
	int lp, up, flag;
	int n, i = 0, m = 0, w = 0;
	double val, d;
	FILE* fp;
	char name[10];
	printf("Enter the input file name: ");
	scanf("%s", name);
	fp = fopen(name, "r");
        while(!feof(fp))
        {
                fscanf(fp, "%d%d", &point[i].x, &point[i].y); // putting the file cordinates in an array
                i++;
        }
        fclose(fp);

	n = sort(point, i-1);

	printf("\nSorted coordinates based on x coordinates:\n "); //After sorting
    display(point, n);

	left.x=point[0].x;
	left.y=point[0].y;
	right.x=point[n-1].x;
    right.y=point[n-1].y;

	lower[w].x = upper[m].x = left.x;
    lower[w].y = upper[m].y = left.y;

	for(i=1;i<n-1;i++)  //store coordinate greater than x of lowest
        {
                val = find_val(left, right, point[i]);
                if(val < 0)
                {
                        upper[m].x=point[i].x;
                        upper[m].y=point[i].y;
                        m++;
                }
                else if(val > 0)
                {
                        lower[w].x=point[i].x;  //store coordinate less than x of lowest
                        lower[w].y=point[i].y;
                        w++;
                }
        }
	lower[w].x = upper[m].x = right.x;
	lower[w].y = upper[m].y = right.y;
	w++;
	m++;

	printf("\nUpper half: "); // display the upper sorted cordinates
	display(upper, m);

	printf("\nLower half:"); // display the lower sorted cordinates
	display(lower, w);

	lower_hull[0].x = left.x;
	lower_hull[0].y = left.y;

	for(i = 0, lp = 1; i < w; ++i, ++lp)
	{
		lower_hull[lp].x = lower[i].x;
		lower_hull[lp].y = lower[i].y;
		// in the lower hull, if area of triangle formed is less than 0 put the point into the hull
		while(lp >=2 && (d = is_convex(lower_hull[lp-2], lower_hull[lp-1], lower_hull[lp])) < 0)
		{
			lower_hull[lp-1].x = lower_hull[lp].x;
			lower_hull[lp-1].y = lower_hull[lp].y;
			lp--;
		}
	}
	printf("\nLower hull: "); // display the lower hull
	display(lower_hull, lp);

	upper_hull[0].x = left.x;
        upper_hull[0].y = left.y;

        for(i = 0, up = 1; i < m; ++i, ++up)
        {
                upper_hull[up].x = upper[i].x;
                upper_hull[up].y = upper[i].y;
		// in the upper hull, if area of triangle formed is more than 0 put the point into the hull
                while(up >=2 && (d = is_convex(upper_hull[up-2], upper_hull[up-1], upper_hull[up])) > 0)
                {
                        upper_hull[up-1].x = upper_hull[up].x;
                        upper_hull[up-1].y = upper_hull[up].y;
                        up--;
                }
        }
        printf("\nUpper hull: "); // display the upper hull
        display(upper_hull, up);

	i = merging(hull, lower_hull, upper_hull, lp, up); // join the upper and lower hull

	printf("\nThe complete hull is: "); // display the entire hull
	display(hull, i);
	flag = check_hull(hull, point, n, i);
	if(flag == 0)
		printf("\nThe hull is wrong.\n");
	else
		printf("\nThe hull is correct.\n");
	return 0;
}
