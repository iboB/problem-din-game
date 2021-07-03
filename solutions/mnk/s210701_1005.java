//j package com.company;

import java.util.*;

/*j public */ class Din {

    static class Triple {
        int a;
        int b;
        int c;

        Triple prev;
        String move;

        public Triple(int a, int b, int c) {
            this.a = a;
            this.b = b;
            this.c = c;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Triple triple = (Triple) o;
            return a == triple.a && b == triple.b && c == triple.c;
        }

        @Override
        public int hashCode() {
            return Objects.hash(a, b, c);
        }
    }

    static HashSet<Triple> visited = new HashSet<>();
    static String[] table = new String[]{"A", "B", "C"};
    static String sol = "";

    static void bfs(int a, int b, int c) {
        Queue<Triple> q = new ArrayDeque<>();
        Triple t = new Triple(a, b, c);
        q.add(t);
        visited.add(t);
        while (!q.isEmpty()) {
            Triple curr = q.poll();
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {

                    int[] arr = new int[]{curr.a, curr.b, curr.c};

                    if (arr[i] == 0) {

                        while (curr != null) {
                            if(curr.move != null)
                                sol = curr.move + "\n" + sol;
                            curr = curr.prev;
                        }
                        return;
                    }

                    if (i == j) {
                        continue;
                    }

                    if (arr[i] >= arr[j]) {
                        arr[i] -= arr[j];
                        arr[j] *= 2;
                        Triple triple = new Triple(arr[0], arr[1], arr[2]);
                        triple.prev = curr;
                        triple.move = table[i] + "->" +
                                table[j];
                        if (!visited.contains(triple)) {
                            q.add(triple);
                        }
                    }
                }
            }
        }
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int a = in.nextInt();
        int b = in.nextInt();
        int c = in.nextInt();
        bfs(a, b, c);
        if (sol.equals("")) {
            System.out.println("NOT POSSIBLE");
        } else {
            System.out.print(sol);
        }
    }
}
