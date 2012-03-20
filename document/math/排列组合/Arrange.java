import java.util.ArrayList;

public class Arrange {   
	  
    private int total = 0;   
    private ArrayList<String> arrangeList = new ArrayList<String>();   
  
    public Arrange() {   
    }   
  
    private void swap(String list[], int k, int i) {   
        String c3 = list[k];   
        list[k] = list[i];   
        list[i] = c3;   
    }   
  
    public void perm(String list[], int k, int m) {   
        if (k > m) {   
            StringBuffer sb = new StringBuffer();   
            for (int i = 0; i <= m; i++) {   
                sb.append(list[i]).append(",");   
            }   
            if (sb.length()>0) {   
                sb.setLength(sb.length()-1);   
            }   
            arrangeList.add(sb.toString());   
            total++;   
        } else {   
            for (int i = k; i <= m; i++) {   
                swap(list, k, i);   
                perm(list, k + 1, m);   
                swap(list, k, i);   
            }   
        }   
    }   
  
    public int getTotal() {   
        return total;   
    }   
       
    public ArrayList<String> getArrangeList() {   
        return arrangeList;   
    }   
  
    public static void main(String args[]) {   
        String list[] = { "1", "2", "3", "4" };   
        Arrange ts = new Arrange();   
        ts.perm(list, 0, list.length-1);   
        for (int i = 0; i < ts.getArrangeList().size(); i++) {   
            System.out.println(ts.getArrangeList().get(i));   
        }   
        System.out.println("total:" + ts.total);   
    }   
  
}  