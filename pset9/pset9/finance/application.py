import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio = []
    records = db.execute(
        "SELECT company, symbol, SUM(shares), SUM(total) FROM purchases GROUP BY company HAVING user_id = ?", session["user_id"])
    valueTotal = 0

    for r in records:
        current = int(lookup(r["symbol"])['price'])
        currentTotal = current * r["SUM(shares)"]
        temp = {
            "company": r["company"],
            "shares": r["SUM(shares)"],
            "avgPrice": usd(int(r["SUM(total)"] / r["SUM(shares)"])),
            "current": usd(current),
            "currentTotal": usd(currentTotal),
            "symbol": r["symbol"]
        }

        valueTotal += int(currentTotal)
        portfolio.append(temp)

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    gTotal = valueTotal + cash
    totals = {"cash": usd(cash), "valueTotal": usd(valueTotal), "gTotal": usd(gTotal)}
    return render_template("index.html", portfolio=portfolio, totals=totals)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Check for method
    if request.method == "POST":
        # Get stock info
        stock = lookup(request.form.get("symbol"))
        # If there is no result return apology
        try:
            int(request.form.get("shares"))
        except ValueError:
            return apology("Not valid shares", 400)

        if not stock:
            return apology("No Search Result", 400)
        # If the share input is negative value return apology
        elif int(request.form.get("shares")) < 0:
            return apology("Not valid shares", 400)

        # Set up variables
        shares = int(request.form.get("shares"))
        price = stock["price"]
        budget = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]

        # If the price is larger than the budget return apology
        if price * shares > budget:
            return apology("Not enough cash!", 400)

        # Add purchase to purchases table
        db.execute("INSERT INTO purchases (user_id, company, price, shares, symbol, total) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], stock["name"], price, shares, request.form.get("symbol"), price * shares)

        # Update budget from users table
        db.execute("UPDATE users SET cash = ? WHERE id = ?", budget - (price * shares), session["user_id"])

        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get all transactions
    portfolio = []
    records = db.execute("SELECT company, symbol, shares, time, price FROM purchases WHERE user_id = ?", session["user_id"])

    # Set up portfolio
    for r in records:
        actionType = "bought" if (r["shares"] > 0) else "sold"
        temp = {
            "company": r["company"],
            "time": r["time"],
            "shares": abs(r["shares"]),
            "type": actionType,
            "price": r["price"]
        }

        portfolio.append(temp)

    return render_template("history.html", portfolio=portfolio)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # Check for method
    if request.method == "POST":
        # Get stock data
        quoted = lookup(request.form.get("symbol"))

        # Return data when exists
        if quoted:
            return render_template("quoted.html", quoted=quoted)
        # Return apology when data doesn't exist
        else:
            return apology("No Search Result", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        # Ensure username doesn't already exist
        elif db.execute("SELECT username FROM users WHERE username=?", request.form.get("username")):
            return apology("the username is taken", 400)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        # Ensure password confirmation is a match
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("password confirmation doesn't match", 400)

        username = request.form.get("username")
        hashed = generate_password_hash(request.form.get("password"))

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashed)

        userId = db.execute("SELECT id FROM users WHERE username=?", request.form.get("username"))[0]['id']
        session["user_id"] = userId

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Check for method
    if request.method == "POST":
        # Get input to sell
        symbolToSell = request.form.get("symbol")
        sharesToSell = int(request.form.get("shares"))
        # Get shares owned
        sharesOwned = db.execute("SELECT SUM(shares) FROM purchases GROUP BY company HAVING user_id = ? AND symbol = ?",
                                 session["user_id"], symbolToSell)[0]["SUM(shares)"]

        # If there is no symbol or user doen't own any of the stock return apology
        if not sharesOwned:
            return apology("You have no such stock", 400)
        # If the share input is negative value return apology
        elif sharesOwned < sharesToSell:
            return apology("You don't have enough shares", 400)

        # Get current price
        stock = lookup(request.form.get("symbol"))
        price = stock["price"]

        # Add purchase to purchases table
        db.execute("INSERT INTO purchases (user_id, company, price, shares, symbol, total) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], stock["name"], price, -sharesToSell, request.form.get("symbol"), -price * sharesToSell)

        # Update budget from users table
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", price * sharesToSell, session["user_id"])

        # Redirect user to home page
        return redirect("/")
    else:
        portfolio = []
        records = db.execute("SELECT symbol, SUM(shares) FROM purchases GROUP BY company HAVING user_id = ?", session["user_id"])

        for r in records:
            temp = {
                "symbol": r["symbol"],
                "shares": r["SUM(shares)"],
            }
            portfolio.append(temp)

        return render_template("sell.html", portfolio=portfolio)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
