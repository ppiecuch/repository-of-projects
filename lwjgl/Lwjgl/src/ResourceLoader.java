



import java.io.InputStream;
import java.net.URL;


/**
 * A small package resource loader
 * 
 * @author xindon
 */
public class ResourceLoader {

	/***************************************************************************************************************************************************************************************************
	 * loadResource() - Get the resource as URL
	 **************************************************************************************************************************************************************************************************/
	public URL loadResource(String resource) {

		if (resource.startsWith("/"))
			resource = resource.substring(1);
		
		while(resource.indexOf("\\") != -1)
			resource = resource.substring(0, resource.indexOf("\\")) + "/" + resource.substring(resource.indexOf("\\") + 1);

		return this.getClass().getResource(resource);
	}


	/***************************************************************************************************************************************************************************************************
	 * loadResourceAsStream() - Get the resource as InputStream
	 **************************************************************************************************************************************************************************************************/
	public InputStream loadResourceAsStream(String resource) {

		try {
			return this.loadResource(resource).openStream();
		} catch (Exception e) {
			return null;
		}

	}
}
