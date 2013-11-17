using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class I18NManager
{
    private static I18NManager instance = null;
    public static I18NManager getInstance(){
        if (instance == null)
            instance = new I18NManager();
        return instance;
    }
    private I18NManager(){
        texts = new Dictionary<uint, string>();
    }

    private Dictionary<uint, string> texts = null;

    public void push(uint id, string text)
    {
        if (texts.ContainsKey(id))
            throw new Exception("already");
        texts.Add(id, text);
    }

    public string getString(uint id)
    {
        string text = null;
        texts.TryGetValue(id, out text);
        return text;
    }

    public string getString(uint id,params object[] args)
    {
        string text = getString(id);
        if (text == null)
            return null;
        try{
            string result = string.Format(text, args);
            return result;
        }catch(Exception e){
            System.Console.WriteLine(e);
        }
        return null;
    }
}

