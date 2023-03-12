# BDSpyrunnerW

![Liscense](https://img.shields.io/github/license/WillowSauceR/BDSpyrunnerW?style=for-the-badge)
[![Downloads](https://img.shields.io/github/downloads/WillowSauceR/BDSpyrunnerW/total?style=for-the-badge)](https://github.com/WillowSauceR/BDSpyrunnerW/releases/latest)
[![DwonloadsLatest](https://img.shields.io/github/downloads/WillowSauceR/BDSpyrunnerW/latest/total?label=DOWNLOAD@LATEST&style=for-the-badge)](https://github.com/WillowSauceR/BDSpyrunnerW/releases/latest)
[![BDS](https://img.shields.io/badge/BDS-1.19.63.01-blue?style=for-the-badge)](https://www.minecraft.net/download/server/bedrock)

简体中文 | [English](README_EN.md)

## 注意！此分支基于主分支Release1.8.7开发而成，API和Listener都没有主分支丰富，使用时请注意您的插件使用的监听器和API

## 简介

``BDSpyrunnerW``是一个用于``BDS``(Bedrock Dedicated Server)的插件加载器。
它使用了 CPython 和 Detours，来连接Python与BDS，使得Python可以操作Minecraft中的一些事物。
极大的降低了插件开发门槛，有效简化了插件的开发成本。

## 用法

1. 下载``Release``中的``BDSpyrunnerW.dll``、``mc.py``和``BDSpyrunnerW.pdb``，pdb文件不是必须的，但是在崩溃时用于反馈会给我们带来很大便利
2. 确保您已经安装了[LiteLoader](https://github.com/LiteLDev/LiteLoaderBDS)或[BDXCore](https://github.com/jfishing/BDXCore)
3. 下载安装[Python3.7.9](https://www.python.org/ftp/python/3.7.9/python-3.7.9-amd64.exe)，安装时勾选 ``Add Python to Path``
4. 将``BDSpyrunnerW.dll``和``BDSpyrunnerW.pdb``放入您用于存放插件的文件夹，如``plugins``、``bdxcore_mod``
5. 将``mc.py``放入``plugins/py``文件夹

## 文档

* [这里](https://pyr.jfishing.love/zh_Hans/)

## 联系我们

* 遇到问题请提交issue
