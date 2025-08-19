📦 Main Parts of the Program
🔹 1. Event (Building Block)

Think of Event as a page in your diary.
It stores:

📝 Name → "Birthday Party"

📅 Date → "25-08-2025"

⏰ Time → "18:00"

🏷️ Type → "Personal"

📍 Location → "Home"

👉 Each page can be:

Written (saved in file as CSV)

Read (shown to user)

🔹 2. EventManager (The Secretary 👩‍💼)

This is like your secretary who manages all pages/events.
It can:

✅ Add new events

🗑️ Delete old ones

🔍 Search quickly

📊 Show a timeline of the whole day

✏️ Edit event details

💾 Save everything in a file events.csv

🔹 3. Modes (Admin vs User)

The program has two doors 🚪:

Admin Mode (VIP 👑)

Needs login (username: vinayak, password: pccoepune)

Can Add, Delete, Edit, Search, View Timeline

User Mode (Guest 🙋)

Can only View, Search, and See Timeline

🎬 How It Works (Example Story)

Program starts → loads diary from events.csv.

It asks: "Are you admin?"

If yes → login required.

If no → enter as user.

Suppose you are admin:

You add event → "Cricket Match, 21-08-2025, 16:00, Sports, Stadium"

Now CSV file saves this event.

Later, as a user:

You search "cricket" → it instantly shows that event.

If you check Timeline for 21-08-2025 →

08:00 |  
09:00 |  
16:00 | Cricket Match
17:00 |  


Looks like a proper schedule!

✨ Why It’s Smart?

No clashes → warns you if another event is at same date+time.

Saves data permanently in CSV → nothing is lost when program ends.

Clean menus → easy for both Admin & User.

📌 One-Line Pitch to Explain:
“This program is like a smart diary 📖 with two modes – Admin controls everything, and Users can only view. Events are saved in a CSV file so they stay safe forever.”
