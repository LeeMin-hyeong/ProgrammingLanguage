package Asignment1;
import java.util.Arrays;
import java.util.Scanner;

public class Asignment1{
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n;
        while(true){
            System.out.printf("Input the number of numbers to process: ");
            n = sc.nextInt();
            if(n>1 && n<31) break;
        }
        int num[] = new int[n];
        System.out.println("Input the numbers to be processed:");
        for(int i=0; i<n; i++){
            num[i] = sc.nextInt();
            if(num[i]<=0 || num[i]>100000){
                System.out.println("Input dismissed");
                i = 0;
                continue;
            }
        }
        long start = System.currentTimeMillis();
        Boolean prime[] = new Boolean[100001];
        Arrays.fill(prime, true);
        prime[1] = false;
        for(int i=2; i<100001; i++)
            for(int j=2; i*j<100001; j++)
                if(prime[i]) prime[i*j] = false;
        Arrays.sort(num);
        for(int i=0; i<n-1; i++){
            System.out.printf("Number of prime numbers between %d %d: ", num[i], num[i+1]);
            int cnt = 0;
            for(int j=num[i]; j<=num[i+1]; j++)
                if(prime[j]) cnt++;
            System.out.println(cnt);
        }
        long end = System.currentTimeMillis();
        System.out.printf("Total execution time using java is %f seconds!\n", (double)(end-start)/1000);
        sc.close();
    }
}