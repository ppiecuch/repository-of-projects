using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class I18NManager : Observable
{
    private class Correspond : MutableResourcable<int>
    {
        public readonly string Name;
        public readonly SystemLanguage Language;
        public readonly string PrefabPath;

        public Correspond(string name,SystemLanguage language,string prefabPath){
            this.Name = name;
            this.Language = language;
            this.PrefabPath = prefabPath;
        }

        public override Resource getResource(int size){
            return new Resource(Resource.Type.Font,PrefabPath+size.ToString());
        }

        public override string ToString()
        {
            return "{" + Name + "," + Language + "," + PrefabPath + "}";
        }
    }

    private static I18NManager instance = null;
    public static I18NManager getInstance(){
        if (instance == null)
            instance = new I18NManager();
        return instance;
    }
    private I18NManager()
    {
        this.dirty = true;
        this.corresponds = new List<Correspond>();
        this.texts = new Dictionary<uint, string>();
        this.fonts = new Dictionary<int, UIFont>();

        this.corresponds.Add(new Correspond("English", SystemLanguage.English, "Prefabs/I18N/English"));
        this.corresponds.Add(new Correspond("Chinese", SystemLanguage.Chinese, "Prefabs/I18N/Chinese"));
    }

    private bool dirty = false;
    private List<Correspond> corresponds = null;
    private Dictionary<uint, string> texts = null;
    private Correspond currentCorrespond = null;
    private Dictionary<int, UIFont> fonts = null;

    public void setLanguage(SystemLanguage language)
    {
        //if (this.currentCorrespond != null)
        //    throw new Exception("already");

        Correspond cp = this.corresponds.Find(
            delegate(Correspond p){
                return p.Language == language;
        });

        if (cp == null)
            throw new Exception("not found");

        this.currentCorrespond = cp;
        this.dirty = true;
    }

    public void reset()
    {
        texts.Clear();
        fonts.Clear();
        //TODO push

        this.dirty = false;

        setChanged();
        notifyObservers();
    }

    //TODO private
    public void push(uint id, string text)
    {
        if (texts.ContainsKey(id))
            throw new Exception("already");
        //TODO check format
        texts.Add(id, text);
    }

    public UIFont getFont(int size){
        if(size<0)
            throw new Exception("size<0");
        if (currentCorrespond == null)
            throw new Exception("setLanguage first!");
        if (dirty)
            throw new Exception("reset first!");
        UIFont font = null;
        this.fonts.TryGetValue(size, out font);
        if(font==null)
        {
            currentCorrespond.setMutableValue(size);
            font = ResourceManager.getInstance().load(currentCorrespond, null).getGameObject().GetComponent<UIFont>();
            this.fonts.Add(size, font);
            Logger.warn(Module.Framework, "add new font:" + currentCorrespond);
        }
        return font;
    }

    public string getString(uint id)
    {
        if (currentCorrespond == null)
            throw new Exception("setLanguage first!");
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
            throw e;
        }
    }
}

