import tkinter as tk
from tkinter import messagebox
from tkinter import filedialog
from tkinter import simpledialog

import subprocess
import os
def scroll_link():
    # update numbers
    update_line_numbers()
    
    # sync scroll
    top_fraction, _ = text.yview()
    line_numbers.yview_moveto(top_fraction)
    root.after(5, scroll_link)

def open_file():
    global current_file
    filename = filedialog.askopenfilename(
        title="Open file",
        filetypes=(("Text files", "*.txt"), ("All files", "*.*"))
    )
    if filename:
        with open(filename, "r") as f:
            content = f.read()
        text.delete("1.0", "end")
        text.insert("1.0", content)
        current_file = filename  # remember this file


def save_file_as():
    global current_file
    filename = filedialog.asksaveasfilename(
        title="Save file",
        defaultextension=".txt",
        filetypes=(("Text files", "*.txt"), ("All files", "*.*"))
    )
    
    if filename:
        try:
            content = text.get("1.0", "end-1c")
            with open(filename, "w") as f:
                f.write(content)
            current_file = filename  # remember this path
            messagebox.showinfo("Saved", f"File saved as {filename}")
        except Exception as e:
            messagebox.showerror("Error", f"Could not save file:\n{e}")


def save_file():
    global current_file
    if current_file:  # If a file is already open, just overwrite
        try:
            content = text.get("1.0", "end-1c")
            with open(current_file, "w") as f:
                f.write(content)
            messagebox.showinfo("Saved", f"File saved: {current_file}")
        except Exception as e:
            messagebox.showerror("Error", f"Could not save file:\n{e}")
    else:
        save_file_as()  # Otherwise act like Save As

def new_file():
    if messagebox.askyesno("New File", "Doy you want to create a new file? Unsaved work will be lost."):
        text.delete("1.0", "end")

def update_emu():
    with open(r"C:\Users\Public\Documents\Breadbox IDE\emucfg.txt", "w") as f:
        content = (
            f"{reg.get()}-"
            f"{speed.get()}-"
            f"{lang.get()}-"
            f"{ctf.get()}"
        )
        f.write(content)

def set_speed():
    value = simpledialog.askinteger(
        "Set Speed",
        "Delay per instruction (ms)",
        minvalue=0,
        maxvalue=10000
    )
    speed.set(value)
    update_emu()   
def emulate():
    save_file()
    update_emu()
    
    # 1. Write the temp file
    # We use the absolute path to ensure C++ finds it
    temp_path = r"C:\Users\Public\Documents\Breadbox IDE\temp.txt"
    
    # Ensure directory exists just in case
    os.makedirs(os.path.dirname(temp_path), exist_ok=True)
    
    with open(temp_path, "w") as f:
        content = text.get("1.0", "end-1c")
        f.write(content)

    # 2. Launch the EXE in a NEW Terminal Window
    if os.path.exists(exe_path):
        # We use Popen instead of run so the Python GUI doesn't freeze
        # 'start cmd /k' opens a new window and keeps it open after the program finishes
        # so you can read the output (cout).
        
        # cmd /c = run command and close (good for finished products)
        # cmd /k = run command and stay open (good for debugging/development)
        
        command = f'start "Breadbox Emulator" cmd /k "{exe_path}"'
        
        # shell=True is required to use the 'start' command
        subprocess.Popen(command, shell=True)
        
    else:
        messagebox.showerror("Error", f"Emulator executable not found at:\n{exe_path}")
# ---------------- Global Variables ----------------
current_file = None  # global variable to store last opened/saved file

root = tk.Tk()
root.title("BreadBox IDE")
icon = tk.PhotoImage(file=r"assets/logo.png")  # replace with your file
root.iconphoto(False, icon)

exe_path = r"C:\Users\Public\Documents\Breadbox IDE\BB24-1 Emulator.exe"
exe_dir = os.path.dirname(r"emulation\BB24-1 Emulator.exe")

cpu_type = tk.IntVar(value=1)  # CPU selection
dlmode = tk.IntVar(value=0)# 0 = Light, 1 = Dark

reg = tk.IntVar(value=1)
speed = tk.IntVar(value=1)
lang = tk.IntVar(value=1)
ctf = tk.IntVar(value=1)

show_lines = tk.BooleanVar(value=True)  # Show line numbers

# ---------------- Close Handling ----------------
def on_closing():
    if messagebox.askyesno("Exit", "Do you really want to close?"):
        root.destroy()

root.protocol("WM_DELETE_WINDOW", on_closing)

# ---------------- Dark/Light Mode ----------------
def apply_mode():
    if dlmode.get() == 0:  # Light Mode
        root.config(bg="white")
        text.config(bg="white", fg="black", insertbackground="black")
        line_numbers.config(bg="lightgrey", fg="black")
    else:  # Dark Mode
        root.config(bg="black")
        text.config(bg="black", fg="white", insertbackground="white")
        line_numbers.config(bg="grey20", fg="white")

# ---------------- Line Numbers ----------------
frame = tk.Frame(root)
frame.pack(fill="both", expand=True)

line_numbers = tk.Text(frame, width=4, padx=4, takefocus=0, border=0,
                       background="lightgrey", state="disabled")
line_numbers.pack(side="left", fill="y")

text = tk.Text(frame, wrap="none")
text.pack(side="right", fill="both", expand=True)

def update_line_numbers(event=None):
    if not show_lines.get():
        line_numbers.config(state="normal")
        line_numbers.delete("1.0", "end")
        line_numbers.config(state="disabled")
        return

    line_numbers.config(state="normal")
    line_numbers.delete("1.0", "end")
    count = int(text.index('end').split('.')[0])
    for i in range(count):
        line_numbers.insert("end", f"{i}\n")
    line_numbers.config(state="disabled")

text.bind("<KeyRelease>", update_line_numbers)
text.bind("<MouseWheel>", update_line_numbers)
text.bind("<Button-1>", update_line_numbers)
text.bind("<Return>", update_line_numbers)

update_line_numbers()  # Initial numbering

# ---------------- Menu Bar ----------------
menu_bar = tk.Menu(root)
root.config(menu=menu_bar)

# File Menu
file_menu = tk.Menu(menu_bar, tearoff=0)
file_menu.add_command(label="New", command=new_file)
file_menu.add_command(label="Open", command=open_file)
file_menu.add_separator()
file_menu.add_command(label="Save", command=save_file)
file_menu.add_command(label="Save As", command=save_file_as)
file_menu.add_separator()
file_menu.add_command(label="Exit", command=on_closing)
menu_bar.add_cascade(label="File", menu=file_menu)

# Preferences Menu
pref_menu = tk.Menu(menu_bar, tearoff=0)

# Dark/Light Mode submenu
theme_menu = tk.Menu(pref_menu, tearoff=0)
theme_menu.add_radiobutton(label="Light Mode", variable=dlmode, value=0, command=apply_mode)
theme_menu.add_radiobutton(label="Dark Mode", variable=dlmode, value=1, command=apply_mode)
pref_menu.add_cascade(label="Theme", menu=theme_menu)

# Show line numbers toggle
pref_menu.add_checkbutton(label="Show Line Numbers", variable=show_lines, command=update_line_numbers)

menu_bar.add_cascade(label="Preferences", menu=pref_menu)

# Compiler Menu
compiler_menu = tk.Menu(menu_bar, tearoff=0)
cpu_menu = tk.Menu(compiler_menu, tearoff=0)
cpu_menu.add_radiobutton(label="BB24-1", variable=cpu_type, value=1, command=lambda: print("CPU 1 selected"))
compiler_menu.add_cascade(label="CPU Type", menu=cpu_menu)

langin_menu = tk.Menu(compiler_menu, tearoff=0)
langin_menu.add_radiobutton(label="Assembly", variable=lang, value=1, command=update_emu)

#langin_menu.add_radiobutton(label="C", variable=lang, value=2, command=update_emu)
#langin_menu.add_radiobutton(label="C++", variable=lang, value=3, command=update_emu)     will add these later on
#langin_menu.add_radiobutton(label="Python", variable=lang, value=4, command=update_emu)
compiler_menu.add_cascade(label="Input language", menu=langin_menu)

ctf_menu = tk.Menu(compiler_menu, tearoff=0)
ctf_menu.add_radiobutton(label="Disabled", variable=ctf, value=1, command=update_emu)

#ctf_menu.add_radiobutton(label="Assembly *.txt*", variable=ctf, value=2, command=update_emu)
#ctf_menu.add_radiobutton(label="Binary *.bin*", variable=ctf, value=3, command=update_emu)     will add these later on
compiler_menu.add_cascade(label="Compile to File", menu=ctf_menu)

menu_bar.add_cascade(label="Compiler", menu=compiler_menu)

# Emulation Menu
emu_menu = tk.Menu(menu_bar, tearoff=0)
reg_menu = tk.Menu(compiler_menu, tearoff=0)
reg_menu.add_radiobutton(label="None", variable=reg, value=99, command=update_emu)
reg_menu.add_separator()
reg_menu.add_radiobutton(label="A0", variable=reg, value=0, command=update_emu)
reg_menu.add_radiobutton(label="B0", variable=reg, value=1, command=update_emu)
reg_menu.add_radiobutton(label="C0", variable=reg, value=2, command=update_emu)
reg_menu.add_separator()
reg_menu.add_radiobutton(label="A1", variable=reg, value=3, command=update_emu)
reg_menu.add_radiobutton(label="B1", variable=reg, value=4, command=update_emu)
reg_menu.add_radiobutton(label="C1", variable=reg, value=5, command=update_emu)
reg_menu.add_separator()
reg_menu.add_radiobutton(label="A2", variable=reg, value=6, command=update_emu)
reg_menu.add_radiobutton(label="B2", variable=reg, value=7, command=update_emu)
reg_menu.add_radiobutton(label="C2", variable=reg, value=8, command=update_emu)
reg_menu.add_separator()
reg_menu.add_radiobutton(label="A3", variable=reg, value=9, command=update_emu)
reg_menu.add_radiobutton(label="B3", variable=reg, value=10, command=update_emu)
reg_menu.add_radiobutton(label="C3", variable=reg, value=11, command=update_emu)
emu_menu.add_cascade(label="Show Register", menu=reg_menu)
emu_menu.add_command(label="Emulation Speed", command=set_speed)
menu_bar.add_cascade(label="Emulation", menu=emu_menu)

# Run Menu
run_menu = tk.Menu(menu_bar, tearoff=0)
run_menu.add_command(label="Emulate", command=emulate)
#run_menu.add_command(label="Assemble", command=emulate) this uploads to cpu, will add later on
menu_bar.add_cascade(label="Run", menu=run_menu)

# ---------------- Initialize Mode ----------------
apply_mode()

root.after(5, scroll_link)

root.mainloop()
