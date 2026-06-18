# Git 新项目上传指南

## 一、首次上传（新项目）

### 1. 在 GitHub 创建空仓库

1. 打开 https://github.com/new
2. 填写仓库名（用英文，如 `my-project`）
3. **不要勾选**任何选项（不要 README、不要 .gitignore、不要 License）
4. 点击 **Create repository**
5. 复制仓库地址，如：`https://github.com/你的用户名/my-project.git`

### 2. 在项目文件夹初始化 Git

```bash
cd 你的项目路径
git init
```

### 3. 关联远程仓库

把第 1 步复制的地址粘贴进来：

```bash
git remote add origin https://github.com/你的用户名/my-project.git
```

### 4. 添加文件并提交

```bash
git add .
git commit -m "项目初始化"
```

### 5. 推送到 GitHub

```bash
git push -u origin main
```

> 加了 `-u` 参数后，以后只需 `git push` 即可。

---

## 二、后续更新（已上传过的项目）

改完代码后，在项目文件夹里执行三行命令：

```bash
git add .
git commit -m "描述你改了什么"
git push
```

---

## 三、常用命令速查

| 想做什么 | 命令 |
|----------|------|
| 查看哪些文件被改了 | `git status` |
| 查看具体改了什么 | `git diff` |
| 查看上传历史 | `git log --oneline` |
| 回退到某次提交 | `git reset --hard commit_id` |
| 克隆别人的仓库 | `git clone <仓库地址>` |

---

## 四、注意事项

- 仓库名和文件夹名不要用中文，避免编码问题
- `.gitignore` 文件里列出的文件不会被上传（如编译生成的 `.exe`）
- 每次 `git commit` 前先 `git status` 确认一下改了什么
- 提交信息写清楚做了什么，方便以后查看
