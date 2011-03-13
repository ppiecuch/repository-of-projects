import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLEncoder;
import java.util.Map;

public static String requestURL(String urlString,String urlParams) throws IOException
	{
        // open the connection
		BufferedReader in =null;
		BufferedWriter out =null;
		try
		{
        URL url= new URL(urlString);
		
        URLConnection urlConn = url.openConnection();
        if (urlParams != null && urlParams.length() > 0) {
            // set the fields
            // urlConn.setDoInput(true);
            urlConn.setDoOutput(true);
            // write request to the url connection
            out = new BufferedWriter(new OutputStreamWriter(urlConn.getOutputStream()));
            // format the properties
            out.write(urlParams);
            out.close();
        }
        in = new BufferedReader(new InputStreamReader(urlConn.getInputStream()));
        String line = null;
        StringBuffer buffer = new StringBuffer(4096);
        while ((line = in.readLine()) != null) buffer.append(line + "\n");
        String response = buffer.toString();
        return response;
		} catch (IOException e) {
			throw e;
		} finally{
			if(in!=null)in.close();
			if(out!=null)out.close();
		}
            
	}
	
	@SuppressWarnings("unchecked")
	public static String formatUrl(Map properties) {
        StringBuffer sb = new StringBuffer(4096);
        String localEncoding = new InputStreamReader(System.in).getEncoding();
        for(Object obj:properties.keySet())
        {
        	try {
	        	String key = (String)obj;
	        	String value = (String)properties.get(key);
	        	sb.append(URLEncoder.encode(key,localEncoding));
	            sb.append("=");
	            sb.append(URLEncoder.encode(value,localEncoding));
	            sb.append("&");
        	} catch (Exception e) {
                e.printStackTrace();
                return null;
            }
        }
        return sb.toString();
    }