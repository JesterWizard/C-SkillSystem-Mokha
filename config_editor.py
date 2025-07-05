import tkinter as tk
from tkinter import filedialog, messagebox, scrolledtext
import os
import subprocess

class ConfigManagerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Configuration Manager")
        self.root.geometry("1000x800")
        self.root.configure(bg="#f0f2f5")

        # Set default config file path relative to the application's current working directory
        self.default_config_file_path = os.path.join(os.getcwd(), "include", "Configs", "configs.h")
        self.config_file_path = self.default_config_file_path # Initialize with default file path

        # Initialize project root path based on the default config file assumption
        # This will be updated if a new config file is loaded or user sets it explicitly
        self.project_root_path = self._calculate_default_project_root(self.config_file_path)

        self.original_config_lines = []
        self.config_options = [] # Stores parsed #define lines with their states
        self.canvas_window_id = None # To store the ID of the window created inside the canvas

        self.create_widgets()
        self.load_initial_file() # Attempt to load the default file at startup

    def _calculate_default_project_root(self, config_path):
        """Calculates the default project root assuming configs.h is in include/Configs/"""
        if config_path:
            # Go two directory levels up from the config_file_path's directory
            # This handles cases where config_file_path might be in a sub-sub-directory
            return os.path.abspath(os.path.join(os.path.dirname(config_path), os.pardir, os.pardir))
        return os.getcwd() # Fallback to current working directory

    def create_widgets(self):
        # Configure root grid for responsive layout
        self.root.grid_rowconfigure(0, weight=0) # File frame
        self.root.grid_rowconfigure(1, weight=0) # Project root frame
        self.root.grid_rowconfigure(2, weight=0) # Buttons frame
        self.root.grid_rowconfigure(3, weight=1) # Content area (checkboxes and output log)
        self.root.grid_columnconfigure(0, weight=1) # Checkboxes column
        self.root.grid_columnconfigure(1, weight=1) # Output log column

        # --- File Selection Frame ---
        file_frame = tk.Frame(self.root, bg="#ffffff", bd=2, relief="groove", padx=15, pady=15)
        file_frame.grid(row=0, column=0, columnspan=2, pady=5, padx=20, sticky="ew") # Spans both columns

        tk.Label(file_frame, text="Current Config File:", bg="#ffffff", font=("Inter", 10, "bold")).pack(side="left", padx=(0, 10))
        self.file_path_label = tk.Label(file_frame, text=self.config_file_path, bg="#ffffff", font=("Inter", 10), fg="#3182ce")
        self.file_path_label.pack(side="left", fill="x", expand=True)

        select_btn = tk.Button(file_frame, text="Select File", command=self.select_file,
                               bg="#4299e1", fg="white", font=("Inter", 10, "bold"),
                               activebackground="#3182ce", activeforeground="white", relief="raised", bd=2)
        select_btn.pack(side="right", padx=(10, 0))

        # --- Project Root Frame ---
        project_root_frame = tk.Frame(self.root, bg="#ffffff", bd=2, relief="groove", padx=15, pady=15)
        project_root_frame.grid(row=1, column=0, columnspan=2, pady=5, padx=20, sticky="ew")

        tk.Label(project_root_frame, text="Project Root (for Make):", bg="#ffffff", font=("Inter", 10, "bold")).pack(side="left", padx=(0, 10))
        # Use a StringVar for the project root path entry
        self.project_root_var = tk.StringVar(value=self.project_root_path)
        self.project_root_entry = tk.Entry(project_root_frame, textvariable=self.project_root_var, bg="#f7fafc", font=("Inter", 10), bd=1, relief="solid")
        self.project_root_entry.pack(side="left", fill="x", expand=True, padx=(0, 10))

        set_root_btn = tk.Button(project_root_frame, text="Browse Root", command=self.set_project_root,
                                  bg="#4299e1", fg="white", font=("Inter", 10, "bold"),
                                  activebackground="#3182ce", activeforeground="white", relief="raised", bd=2)
        set_root_btn.pack(side="right")

        # --- Buttons Frame ---
        button_frame = tk.Frame(self.root, bg="#f0f2f5")
        button_frame.grid(row=2, column=0, columnspan=2, pady=10, padx=20, sticky="ew") # Spans both columns

        self.enable_all_btn = tk.Button(button_frame, text="Enable All", command=self.enable_all,
                                        bg="#4299e1", fg="white", font=("Inter", 12, "bold"),
                                        activebackground="#3182ce", activeforeground="white", relief="raised", bd=2)
        self.enable_all_btn.pack(side="left", padx=5, expand=True, fill="x")

        self.disable_all_btn = tk.Button(button_frame, text="Disable All", command=self.disable_all,
                                         bg="#a0aec0", fg="white", font=("Inter", 12, "bold"),
                                         activebackground="#718096", activeforeground="white", relief="raised", bd=2)
        self.disable_all_btn.pack(side="left", padx=5, expand=True, fill="x")

        self.save_btn = tk.Button(button_frame, text="Save", command=self.save_file,
                                  bg="#48bb78", fg="white", font=("Inter", 12, "bold"),
                                  activebackground="#38a169", activeforeground="white", relief="raised", bd=2)
        self.save_btn.pack(side="left", padx=5, expand=True, fill="x")

        self.build_btn = tk.Button(button_frame, text="Build", command=self.build_project,
                                   bg="#4299e1", fg="white", font=("Inter", 12, "bold"),
                                   activebackground="#3182ce", activeforeground="white", relief="raised", bd=2)
        self.build_btn.pack(side="left", padx=5, expand=True, fill="x")

        # --- Checkboxes Section (left column) ---
        # Create a frame to hold the canvas and its scrollbar
        self.canvas_container = tk.Frame(self.root, bg="#ffffff", bd=2, relief="groove") # Changed relief
        self.canvas_container.grid(row=3, column=0, sticky="nsew", padx=(20, 10), pady=10) # Position in grid

        self.canvas = tk.Canvas(self.canvas_container, bg="#ffffff", bd=0, highlightthickness=0) # No border, highlight
        self.canvas.pack(side="left", fill="both", expand=True) # Canvas fills most of its container

        # Scrollbar placed on the right of the canvas, within the canvas_container
        self.scrollbar = tk.Scrollbar(self.canvas_container, orient="vertical", command=self.canvas.yview)
        self.scrollbar.pack(side="right", fill="y")

        self.canvas.configure(yscrollcommand=self.scrollbar.set)
        # Bind configure event to canvas to dynamically adjust the internal frame width
        self.canvas.bind('<Configure>', self._on_canvas_resize)

        # Frame inside the canvas to hold checkboxes
        self.checkboxes_frame = tk.Frame(self.canvas, bg="#ffffff")
        # Create a window in the canvas to contain the checkboxes_frame
        self.canvas_window_id = self.canvas.create_window((0, 0), window=self.checkboxes_frame, anchor="nw", width=self.canvas.winfo_width())
        # Bind scroll events to the checkboxes_frame for more consistent scrolling
        self.checkboxes_frame.bind("<Configure>", lambda e: self.canvas.configure(scrollregion = self.canvas.bbox("all")))


        # --- Output Log Section (right column) ---
        output_frame = tk.LabelFrame(self.root, text="Output Log", bg="#f7fafc", bd=2, relief="groove", font=("Inter", 12, "bold"), fg="#2d3748")
        output_frame.grid(row=3, column=1, sticky="nsew", padx=(10, 20), pady=10) # Position in grid

        self.output_log = scrolledtext.ScrolledText(output_frame, wrap=tk.WORD, height=10, bg="#f7fafc", fg="#2d3748", font=("Consolas", 10), bd=0, padx=10, pady=10)
        self.output_log.pack(fill="both", expand=True)
        self.output_log.insert(tk.END, "Application loaded.\n")
        self.output_log.config(state=tk.DISABLED) # Make it read-only

    def log_message(self, message):
        self.output_log.config(state=tk.NORMAL)
        self.output_log.insert(tk.END, message + "\n")
        self.output_log.see(tk.END) # Scroll to the end
        self.output_log.config(state=tk.DISABLED)

    def _on_canvas_resize(self, event):
        """Adjusts the width of the frame inside the canvas when the canvas resizes."""
        # Update the width of the window inside the canvas to match canvas width
        if self.canvas_window_id: # Ensure the window exists
            self.canvas.itemconfig(self.canvas_window_id, width=event.width)
        # Also update scrollregion after resizing the internal frame
        self.canvas.config(scrollregion=self.canvas.bbox("all"))

    def select_file(self):
        # filedialog.askopenfilename's initialdir should use the current working directory
        # for consistency with the default path logic.
        file_path = filedialog.askopenfilename(
            initialdir=os.getcwd(), # Starts browser in the app's launch directory
            title="Select configs.h",
            filetypes=(("Header files", "configs.h"), ("All files", "*.*"))
        )
        if file_path:
            self.config_file_path = file_path
            self.file_path_label.config(text=self.config_file_path)
            self.project_root_path = self._calculate_default_project_root(self.config_file_path)
            self.project_root_var.set(self.project_root_path) # Update entry field
            self.load_config_file()

    def set_project_root(self):
        """Allows user to explicitly set the project root directory."""
        # Use os.getcwd() as initialdir if current project_root is not valid
        initial_dir = self.project_root_path if os.path.isdir(self.project_root_path) else os.getcwd()
        folder_path = filedialog.askdirectory(
            initialdir=initial_dir,
            title="Select Project Root Directory (where Makefile is)"
        )
        if folder_path:
            self.project_root_path = folder_path
            self.project_root_var.set(self.project_root_path) # Update entry field
            self.log_message(f"Project Root set to: {self.project_root_path}")


    def load_initial_file(self):
        """Attempts to load the default configs.h file at startup."""
        if os.path.exists(self.default_config_file_path):
            self.load_config_file()
        else:
            self.log_message(f"Default file not found at: {self.default_config_file_path}")
            self.log_message("Please select 'configs.h' using the 'Select File' button.")

    def load_config_file(self):
        try:
            with open(self.config_file_path, 'r', encoding='utf-8') as f:
                self.original_config_lines = f.readlines()
            self.parse_config_content()
            self.render_checkboxes()
            self.log_message(f"Successfully loaded: {self.config_file_path}")
        except FileNotFoundError:
            messagebox.showerror("Error", f"File not found: {self.config_file_path}")
            self.log_message(f"Failed to load: {self.config_file_path} (File not found)")
        except Exception as e:
            messagebox.showerror("Error", f"Error loading file: {e}")
            self.log_message(f"Error loading file {self.config_file_path}: {e}")

    def parse_config_content(self):
        self.config_options = []
        for i, line in enumerate(self.original_config_lines):
            stripped_line = line.strip()

            # Check for #define lines
            if stripped_line.startswith('#define '):
                define_name = stripped_line
                is_enabled = True
                self.config_options.append({
                    'index': i,
                    'original_line': line,
                    'is_define': True,
                    'is_enabled': is_enabled,
                    'define_name': define_name,
                    'is_commented_define': False,
                    'section_header': None # Track section for grouping
                })
            elif stripped_line.startswith('// #define '):
                define_name = stripped_line[3:] # Remove '// '
                is_enabled = False
                self.config_options.append({
                    'index': i,
                    'original_line': line,
                    'is_define': True,
                    'is_enabled': is_enabled,
                    'define_name': define_name,
                    'is_commented_define': True,
                    'section_header': None
                })
            # Check for section headers (heuristics)
            elif stripped_line.startswith('/*') and stripped_line.endswith('*/') and len(stripped_line) > 4:
                 self.config_options.append({
                    'index': i,
                    'original_line': line,
                    'is_define': False,
                    'is_enabled': False,
                    'define_name': None,
                    'is_commented_define': False,
                    'section_header': stripped_line.strip('/*').strip('*/').strip()
                })
            elif stripped_line.startswith('// By '):
                 self.config_options.append({
                    'index': i,
                    'original_line': line,
                    'is_define': False,
                    'is_enabled': False,
                    'define_name': None,
                    'is_commented_define': False,
                    'section_header': stripped_line[3:].strip()
                })
            elif stripped_line.startswith('/**') and stripped_line.endswith('*/'):
                 self.config_options.append({
                    'index': i,
                    'original_line': line,
                    'is_define': False,
                    'is_enabled': False,
                    'define_name': None,
                    'is_commented_define': False,
                    'section_header': stripped_line.strip('/**').strip('*/').strip()
                })
            else:
                self.config_options.append({
                    'index': i,
                    'original_line': line,
                    'is_define': False,
                    'is_enabled': False,
                    'define_name': None,
                    'is_commented_define': False,
                    'section_header': None
                })

    def render_checkboxes(self):
        # Clear existing widgets in the frame
        for widget in self.checkboxes_frame.winfo_children():
            widget.destroy()

        current_checkbox_group = None

        for option in self.config_options:
            if option['section_header']:
                # Create a new section title and a new frame for checkboxes
                section_title_label = tk.Label(self.checkboxes_frame, text=option['section_header'],
                                               font=("Inter", 14, "bold"), bg="#ffffff", fg="#2d3748", anchor="w")
                section_title_label.pack(fill="x", padx=10, pady=(15, 5))

                current_checkbox_group = tk.Frame(self.checkboxes_frame, bg="#ffffff")
                current_checkbox_group.pack(fill="x", padx=10, pady=5)
                # Use grid for better organization within the group
                # Each row will have one checkbox item
                current_checkbox_group.grid_columnconfigure(0, weight=1)

            elif option['is_define']:
                if not current_checkbox_group:
                    # If no section header was found yet, create a default group
                    current_checkbox_group = tk.Frame(self.checkboxes_frame, bg="#ffffff")
                    current_checkbox_group.pack(fill="x", padx=10, pady=5)
                    current_checkbox_group.grid_columnconfigure(0, weight=1)


                # Extract the actual define name without '#define' or values
                display_name = option['define_name'].replace('#define', '').strip()
                if ' ' in display_name:
                    display_name = display_name.split(' ')[0] # Get only the macro name

                var = tk.BooleanVar(value=option['is_enabled'])
                var.trace_add("write", lambda name, index, mode, var=var, opt=option: self.on_checkbox_change(var, opt))

                # Use a Frame to create a visually appealing checkbox item
                item_frame = tk.Frame(current_checkbox_group, bg="#f7fafc", bd=1, relief="solid")
                item_frame.pack(fill="x", pady=2, padx=5) # pack in the item_frame
                item_frame.bind("<Button-1>", lambda e, cb=var: cb.set(not cb.get())) # Toggle on frame click


                checkbox = tk.Checkbutton(item_frame, text=display_name, variable=var,
                                          bg="#f7fafc", selectcolor="#e2e8f0", fg="#4a5568",
                                          font=("Inter", 10), anchor="w",
                                          command=lambda opt=option: self.on_checkbox_change(var, opt))
                checkbox.pack(side="left", padx=5, pady=5, fill="x", expand=True)

                # Store the Tkinter variable in the option for easy access
                option['var'] = var

        # Update the scroll region after all widgets are placed
        self.root.update_idletasks()
        self.canvas.config(scrollregion=self.canvas.bbox("all"))

    def on_checkbox_change(self, var, option):
        option['is_enabled'] = var.get()

    def enable_all(self):
        for option in self.config_options:
            if option['is_define']:
                option['is_enabled'] = True
                if 'var' in option:
                    option['var'].set(True)
        messagebox.showinfo("Success", "All #define options enabled.")
        self.log_message("All #define options enabled.")

    def disable_all(self):
        for option in self.config_options:
            if option['is_define']:
                option['is_enabled'] = False
                if 'var' in option:
                    option['var'].set(False)
        messagebox.showinfo("Success", "All #define options disabled.")
        self.log_message("All #define options disabled.")

    def save_file(self):
        if not self.config_file_path or not os.path.exists(self.config_file_path):
            messagebox.showwarning("Warning", "No valid file loaded to save.")
            return

        new_lines = self.original_config_lines[:] # Start with a copy of original lines

        for option in self.config_options:
            if option['is_define']:
                current_line = new_lines[option['index']].strip()
                if option['is_enabled'] and option['is_commented_define']:
                    # Remove '// ' prefix
                    new_lines[option['index']] = current_line[3:] + "\n"
                elif not option['is_enabled'] and not option['is_commented_define']:
                    # Add '// ' prefix
                    new_lines[option['index']] = "// " + current_line + "\n"
                # If already in desired state, do nothing
            # Non-define lines are kept as is

        try:
            with open(self.config_file_path, 'w', encoding='utf-8') as f:
                f.writelines(new_lines)
            messagebox.showinfo("Success", f"Configuration saved to {self.config_file_path}")
            self.log_message(f"Configuration saved to: {self.config_file_path}")
            # Reload to refresh internal state based on saved file (important for accurate current_line in next save)
            self.load_config_file()
        except Exception as e:
            messagebox.showerror("Error", f"Error saving file: {e}")
            self.log_message(f"Error saving file {self.config_file_path}: {e}")

    def build_project(self):
        project_root = self.project_root_var.get() # Get the project root from the Entry field

        if not project_root:
            messagebox.showwarning("Warning", "Please specify the Project Root directory where your Makefile is located.")
            self.log_message("Build aborted: Project Root not specified.")
            return

        if not os.path.isdir(project_root):
            messagebox.showwarning("Warning", f"The specified Project Root is not a valid directory: {project_root}")
            self.log_message(f"Build aborted: Invalid Project Root directory: {project_root}")
            return

        self.log_message(f"Attempting to build project from: {project_root}")
        self.log_message("Running command: make -j6")

        try:
            # Use subprocess.Popen for non-blocking execution and to capture output
            # shell=True is often needed for 'make' on Windows or if it's not in PATH
            process = subprocess.Popen(
                ["make", "-j6"],
                cwd=project_root, # Use the user-defined project root
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True, # Use text mode for universal newlines and string output
                shell=True
            )

            # Read output in real-time (or after completion for simplicity here)
            stdout, stderr = process.communicate(timeout=600) # 10 minute timeout

            if stdout:
                self.log_message("\n--- Build Output (STDOUT) ---")
                self.log_message(stdout)
            if stderr:
                self.log_message("\n--- Build Output (STDERR) ---")
                self.log_message(stderr)

            if process.returncode == 0:
                messagebox.showinfo("Build Status", "Build completed successfully!")
                self.log_message("Build completed successfully!")
            else:
                messagebox.showerror("Build Status", f"Build failed with exit code {process.returncode}.")
                self.log_message(f"Build failed with exit code {process.returncode}.")

        except FileNotFoundError:
            messagebox.showerror("Error", "The 'make' command was not found. Please ensure Make is installed and in your system's PATH.")
            self.log_message("Error: 'make' command not found. Ensure Make is installed and in PATH.")
        except subprocess.TimeoutExpired:
            process.kill()
            stdout, stderr = process.communicate()
            messagebox.showerror("Build Status", "Build timed out after 10 minutes.")
            self.log_message("Build timed out.")
            if stdout: self.log_message(stdout)
            if stderr: self.log_message(stderr)
        except Exception as e:
            messagebox.showerror("Error", f"An error occurred during build: {e}")
            self.log_message(f"An error occurred during build: {e}")

if __name__ == "__main__":
    root = tk.Tk()
    app = ConfigManagerApp(root)
    root.mainloop()