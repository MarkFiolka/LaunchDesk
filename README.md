# **LaunchDesk**

This is **LaunchDesk**, a Windows desktop productivity tool built with **C++ and Qt**, developed by Mark Fiolka.  
LaunchDesk helps you start applications, terminal commands, and complete workflows through configurable profiles with a single click.

---

## **Overview**

- Platform: Windows  
- Language: C++  
- Framework: Qt 6  
- Type: Desktop application  

---

## **Core Concept**

LaunchDesk solves the problem of having multiple applications permanently configured in the Windows autostart, where they consume system resources even when not needed.

Instead of autostart, LaunchDesk allows you to define **profiles** for different daily use cases such as gaming, development, or focused work.  
Each profile contains a set of actions that are only started when the profile is explicitly launched.

Supported actions include launching executables, opening URLs, and running terminal commands.  
Terminal commands can be defined using **variables**, making it easy to reuse and adapt commands without rewriting them.

---

## **Profiles**

Profiles group actions for specific use cases (e.g. Gaming, Development, Focus, etc).

- Each profile can launch multiple actions  
- Profiles are independent and configurable  
- No background processes unless a profile is started  

---

## **Variables**

LaunchDesk supports **variables** inside terminal commands and parameters.  
Variables act as placeholders that can be replaced with different values without modifying the original command structure.

## Variable Syntax

"Variables are defined using angle brackets."

**Example command (without variables)**

> New-BurntToastNotification -Text "Todays News", "Its Sunny outside..."

**Example command (with variables)**

> New-BurntToastNotification -Text &lt;Title&gt;, &lt;Text&gt;

**In this example used Variables**

> &lt;Title&gt;: "Todays News"

> &lt;Text&gt;: "Its Sunny outside..."

This allows the same command to be reused while only changing variable values.

---

## **Project Structure**

The Project is structured in "core", "ui" and "resources"

/core        -> Core logic, data models, configuration handling

/ui          -> Windows (the User Interfaces), dialogs, UI helpers

/resources   -> Seperate Stylesheets (QSS) for most Elements, icons (appicon, buttonicons), assets

--- 

## **Project Status**

Finished and Usable

--- 
