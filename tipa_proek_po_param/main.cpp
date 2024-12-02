#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;
class Product {
public:
    int id;
    string name;
    double price;
    int quantity;
    string category;
    Product(int id, string name, double price, int quantity, string category)
        : id(id), name(name), price(price), quantity(quantity), category(category) {
    }
    void displayInfo() const {
        cout << "ID: " << id << ", Name: " << name << ", Price: " << price
            << ", Quantity: " << quantity << ", Category: " << category << endl;
    }
    bool isAvailable() const {
        return quantity > 0;
    }
};
class Category {
public:
    int id;
    string name;
    vector<Product> products;
    Category(int id, string name) : id(id), name(name) {}
    void addProduct(const Product& product) {
        products.push_back(product);
    }
    void removeProduct(int productId) {
        products.erase(
            remove_if(products.begin(), products.end(),
                [productId](const Product& product) { return product.id == productId; }),
            products.end());
    }
};
class Customer {
public:
    int id;
    string name;
    vector<int> purchaseHistory;
    Customer(int id, string name) : id(id), name(name) {}
    void addPurchase(int orderId) {
        purchaseHistory.push_back(orderId);
    }
    void displayPurchaseHistory() const {
        cout << "Purchase history for " << name << ":\n";
        for (int orderId : purchaseHistory) {
            cout << "Order ID: " << orderId << endl;
        }
    }
};
class Order {
public:
    int orderId;
    int customerId;
    vector<Product> products;
    double totalPrice;

    Order(int orderId, int customerId, const vector<Product>& products)
        : orderId(orderId), customerId(customerId), products(products) {
        totalPrice = accumulate(products.begin(), products.end(), 0.0,
            [](double sum, const Product& product) {
                return sum + product.price * product.quantity;
            });
    }
};
class InventoryManager {
public:
    vector<Product> products;

    void addProduct(const Product& product) {
        products.push_back(product);
    }

    void removeProduct(int productId) {
        products.erase(
            remove_if(products.begin(), products.end(),
                [productId](const Product& product) { return product.id == productId; }),
            products.end());
    }

    void displayAllProducts() const {
        cout << "Products in inventory:\n";
        for (const auto& product : products) {
            product.displayInfo();
        }
    }
};
class StoreManager {
public:
    InventoryManager inventory;
    vector<Category> categories;
    vector<Customer> customers;
    vector<Order> orders;
    int nextOrderId = 1;

    void addCategory(const Category& category) {
        categories.push_back(category);
    }
    void registerCustomer(const Customer& customer) {
        customers.push_back(customer);
    }
    void createOrder(int customerId, const vector<int>& productIds) {
        auto customerIt = find_if(customers.begin(), customers.end(),
            [customerId](const Customer& customer) {
                return customer.id == customerId;
            });
        if (customerIt == customers.end()) {
            cout << "Customer not found!\n";
            return;
        }
        vector<Product> orderProducts;
        for (int productId : productIds) {
            auto productIt = find_if(inventory.products.begin(), inventory.products.end(),
                [productId](const Product& product) {
                    return product.id == productId;
                });
            if (productIt != inventory.products.end()) {
                orderProducts.push_back(*productIt);
            }
        }
        if (orderProducts.empty()) {
            cout << "No products found for the order!\n";
            return;
        }
        Order newOrder(nextOrderId++, customerId, orderProducts);
        orders.push_back(newOrder);
        customerIt->addPurchase(newOrder.orderId);
        cout << "Order created! Total price: " << newOrder.totalPrice << endl;
    }
};
int main() {
    StoreManager store;
    Category electronics(1, "Electronics");
    store.addCategory(electronics);
    Product phone(1, "Smartphone", 799.99, 10, "Electronics");
    store.inventory.addProduct(phone);
    electronics.addProduct(phone);
    Product laptop(2, "Laptop", 1199.99, 5, "Electronics");
    store.inventory.addProduct(laptop);
    electronics.addProduct(laptop);
    Customer customer(1, "John Doe");
    store.registerCustomer(customer);
    store.createOrder(1, { 1, 2 });
    store.inventory.displayAllProducts();
    customer.displayPurchaseHistory();
    return 0;
}
