package com.example.myapplication;

/**
 * @author https://github.com/103style
 * @date 2019/8/19 14:54
 */
public class TestClass {

    private String name;

    private String webSite;

    public TestClass() {
    }

    public TestClass(String name, String webSite) {
        this.name = name;
        this.webSite = webSite;
    }

    @Override
    public String toString() {
        return "name='" + name + '\'' +
                ", webSite='" + webSite;
    }
}
