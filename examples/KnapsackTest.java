
class KnapsackTest {
    private static long[] solveKnapsack (int[] weights, long[] values, int maxWeight)
    {
        long[] m_current = new long[maxWeight+1];
        long[] m_previous = new long[maxWeight+1];

        for( int item = 0; item < values.length; item++) {
            for( int w = 0; w < maxWeight + 1; w++) {
                m_current[w] = m_previous[w];
                if( weights[item] <= w ) {
                    if(m_current[w] < m_previous[w-weights[item]] + values[item])
                        m_current[w] = m_previous[w-weights[item]] + values[item];
                }
            }
            m_previous = m_current;
            m_current = new long[maxWeight+1];
        }
        return m_previous;
    }

    public static void main (String[] args) {
        int NUM_ITEMS = 4000;
        int[] weights = new int[NUM_ITEMS];
        long[] values =  new long[NUM_ITEMS];
        int maxWeight = 4000;

        System.out.println(String.format("Set weight: %d, set items number: %d\n", maxWeight, values.length));

        for(int test = 1; test < 11; test++) {
            for(int i = 0; i < NUM_ITEMS; i++) {
                weights[i] = 13*i*test - (i*test*13/1231)*1231;
                values[i] = 17*i*test - (i*test*17/373)*373;
            }

            int mw = maxWeight - (test-1-(test-1)/5*5)*800;
            long[] result = solveKnapsack ( weights, values, mw );
            System.out.print(String.format("Result %d: %d\n", mw, result[mw]));
            doSleep();
        }

        for(int test = 1; test < 11; test++) {
            for(int i = 0; i < NUM_ITEMS; i++) {
                weights[i] = 13*i*test - (i*test*13/1231)*1231;
                values[i] = 17*i*test - (i*test*17/373)*373;
            }

            int mw = 200;
            long[] result = solveKnapsack ( weights, values, mw );
            System.out.print(String.format("Result %d: %d\n", mw, result[mw]));
            doSleep();
        }
    }

    private static void doSleep() {
        try {
            Thread.sleep(2000);
        } catch(InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }
}
