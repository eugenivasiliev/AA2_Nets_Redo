#include "Database.h"

Database::Database() : driver(nullptr), con(nullptr)
{
}

Database::~Database()
{
    Disconnect();
}

bool Database::Connect()
{
    try
    {
        driver = get_driver_instance();
        con = driver->connect(SERVER, USERNAME, PASSWORD);
        con->setSchema(DATABASE);

        std::cout << "Connection done." << std::endl;
        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
        con = nullptr;
        return false;
    }
}

void Database::Disconnect()
{
    if (con == nullptr)
        return;

    con->close();

    if (con->isClosed())
    {
        std::cout << "Connection close." << std::endl;
        delete con;
        con = nullptr;
    }
}

bool Database::UserExists(const std::string& nickname)
{
    try
    {
        std::string query = "SELECT id FROM users WHERE nickname = ?";
        sql::PreparedStatement* stmt = con->prepareStatement(query);

        stmt->setString(1, nickname);

        sql::ResultSet* res = stmt->executeQuery();

        bool exists = res->next();

        delete res;
        delete stmt;

        return exists;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error checking user: " << e.what() << std::endl;
        return false;
    }
}

bool Database::RegisterUser(const std::string& nickname, const std::string& password)
{
    try
    {
        if (UserExists(nickname))
        {
            std::cout << "Register failed: nickname already exists." << std::endl;
            return false;
        }

        // Insert user
        std::string insertUserQuery = "INSERT INTO users (nickname, password_hash) VALUES (?, ?)";
        sql::PreparedStatement* stmtUser = con->prepareStatement(insertUserQuery);

        stmtUser->setString(1, nickname);
        stmtUser->setString(2, password);

        int affectedRows = stmtUser->executeUpdate();
        delete stmtUser;

        if (affectedRows <= 0)
        {
            std::cout << "Register failed: user insert failed." << std::endl;
            return false;
        }

        // Get last inserted ID
        sql::PreparedStatement* stmtId = con->prepareStatement("SELECT LAST_INSERT_ID() AS user_id");
        sql::ResultSet* resId = stmtId->executeQuery();

        int userId = -1;

        if (resId->next())
        {
            userId = resId->getInt("user_id");
        }

        delete resId;
        delete stmtId;

        if (userId == -1)
        {
            std::cout << "Register failed: could not retrieve ID." << std::endl;
            return false;
        }

        // Insert ranking
        std::string insertRankingQuery = "INSERT INTO rankings (points, user_id) VALUES (?, ?)";
        sql::PreparedStatement* stmtRanking = con->prepareStatement(insertRankingQuery);

        stmtRanking->setInt(1, newUserPoints);
        stmtRanking->setInt(2, userId);

        int rankingRows = stmtRanking->executeUpdate();
        delete stmtRanking;

        if (rankingRows <= 0)
        {
            std::cout << "Register failed: ranking insert failed." << std::endl;
            return false;
        }

        std::cout << "User registered successfully. ID: " << userId << std::endl;
        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error registering user: " << e.what() << std::endl;
        return false;
    }
}

bool Database::LoginUser(const std::string& nickname, const std::string& password, int& userId)
{
    userId = -1;

    try
    {
        std::string query = "SELECT id, password_hash FROM users WHERE nickname = ?";
        sql::PreparedStatement* stmt = con->prepareStatement(query);

        stmt->setString(1, nickname);

        sql::ResultSet* res = stmt->executeQuery();

        if (!res->next())
        {
            std::cout << "Login failed: user not found." << std::endl;
            delete res;
            delete stmt;
            return false;
        }

        int dbUserId = res->getInt("id");
        std::string dbPassword = res->getString("password_hash");

        delete res;
        delete stmt;

        if (!bcrypt::validatePassword(password, dbPassword))
        {
            std::cout << "Login failed: wrong password." << std::endl;
            return false;
        }

        userId = dbUserId;

        std::cout << "Login successful. ID: " << userId << std::endl;
        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error logging in: " << e.what() << std::endl;
        return false;
    }
}

bool Database::ModifyScore(const std::string& nickname, int amount)
{
    try
    {
        //Get id
        std::string query = "SELECT id FROM users WHERE nickname = ?";
        sql::PreparedStatement* stmt = con->prepareStatement(query);

        stmt->setString(1, nickname);

        sql::ResultSet* res = stmt->executeQuery();

        if (!res->next())
        {
            std::cout << "Score modification failed: user doesn't exist" << std::endl;
            delete res;
            delete stmt;
            return false;
        }

        int dbUserId = res->getInt("id");

        std::cout << "Retrieved id: " << dbUserId << std::endl;

        delete res;
        delete stmt;

        //Get points
        query = "SELECT points FROM rankings WHERE user_id = ?";
        stmt = con->prepareStatement(query);

        stmt->setInt(1, dbUserId);

        res = stmt->executeQuery();

        if (!res->next())
        {
            std::cout << "Score modification failed: ranking doesn't exist" << std::endl;
            delete res;
            delete stmt;
            return false;
        }

        int points = res->getInt("points");

        std::cout << "Point count: " << points << std::endl;

        delete res;
        delete stmt;

        //Update points
        points += amount;

        query = "UPDATE rankings SET points = ? WHERE user_id = ?";
        stmt = con->prepareStatement(query);

        stmt->setInt(1, points);
        stmt->setInt(2, dbUserId);

        res = stmt->executeQuery();

        delete res;
        delete stmt;

        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error updating score: " << e.what() << std::endl;
        return false;
    }
}

std::vector<User> Database::GetTop10Rankings()
{
    std::vector<User> top10;

    try
    {
        std::string query = "SELECT points, user_id FROM rankings ORDER BY points DESC LIMIT 10";

        sql::PreparedStatement* stmt = con->prepareStatement(query);
        sql::ResultSet* res = stmt->executeQuery();

        int position = 1;
        while (res->next() && position <= 10)
        {
            User entry;
            entry.position = position;
            entry.userIndex = res->getInt("user_id");
            entry.score = res->getInt("points");

            top10.push_back(entry);
            position++;
        }

        delete res;
        delete stmt;

        for (int i = 0; i < top10.size(); i++) {
            query = "SELECT nickname FROM users WHERE id = ?";

            stmt = con->prepareStatement(query);

            stmt->setInt(1, top10[i].userIndex);

            res = stmt->executeQuery();

            if (!res->next())
            {
                std::cout << "Failed: nickname doesn't exist" << std::endl;
                delete res;
                delete stmt;
                continue;
            }

            top10[i].nickname = res->getString("nickname");

            delete res;
            delete stmt;
        }

        std::cout << "Recibido " << top10.size() << " puntuaciones." << std::endl;
        return top10;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error consiguiendo top 10 rankings: " << e.what() << std::endl;
        return top10;
    }


}
