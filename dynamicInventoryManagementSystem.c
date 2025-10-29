#include <stdio.h>
#include <stdlib.h>

// structure for Product
struct Product
{
    int productID;
    char productName[50];
    float price;
    int quantity;
};

// global variable to hold the inventory and size of inventory
struct Product *productInventory;
int inventorySize;

// helepr method to add new product
int addNewProduct()
{
    // increase inventory size
    inventorySize++;
    // reallocate memory for inventory
    // transfer the productInventory pointer to a temporary pointer to avoid memory leak in case of realloc failure
    struct Product *temporaryInventory = NULL;
    if (productInventory == NULL)
        temporaryInventory = (struct Product *)malloc(inventorySize * sizeof(struct Product));
    else
        temporaryInventory = (struct Product *)realloc(productInventory, inventorySize * sizeof(struct Product));

    // check for memory allocation failure
    if (temporaryInventory == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }
    // update the productInventory pointer
    productInventory = temporaryInventory;
    // get the details for new product
    printf("Enter new product details:\n");
    printf("Product ID: ");
    scanf("%d", &productInventory[inventorySize - 1].productID);
    printf("Product Name: ");
    scanf("%s", productInventory[inventorySize - 1].productName);
    printf("Product Price: ");
    scanf("%f", &productInventory[inventorySize - 1].price);
    printf("Product Quantity: ");
    scanf("%d", &productInventory[inventorySize - 1].quantity);

    printf("Product added successfully!\n");
    return 0;
}

// helper method to view the product details
void viewProductDetails(struct Product product)
{
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           product.productID,
           product.productName,
           product.price,
           product.quantity);
}

// helper method to view all products
void viewAllProducts()
{
    printf("========= PRODUCT LIST =========\n");
    for (int productIndex = 0; productIndex < inventorySize; productIndex++)
    {
        viewProductDetails(productInventory[productIndex]);
    }
}

// helper method to update quantity of a product
void updateQuantity()
{
    // get the product id and new quantity
    int updateProductID, newQuantity;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &updateProductID);
    printf("Enter new quantity: ");
    scanf("%d", &newQuantity);
    // product not found flag
    int productNotFound = 1;
    // search for the product and update quantity
    for (int productIndex = 0; productIndex < inventorySize; productIndex++)
    {
        if (productInventory[productIndex].productID == updateProductID)
        {
            productNotFound = 0;
            productInventory[productIndex].quantity = newQuantity;
            printf("Quantity updated successfully!\n");
            return;
        }
    }

    // if product not found
    if (productNotFound)
    {
        printf("Product Not Found!\n");
    }
}

// helper method to search the product by ID
void searchProductByID()
{
    // get the product id to search
    int searchProductID;
    printf("Enter Product ID to search: ");
    scanf("%d", &searchProductID);
    // search the product id in inventory
    for (int productIndex = 0; productIndex < inventorySize; productIndex++)
    {
        if (productInventory[productIndex].productID == searchProductID)
        {
            printf("Product Found:\n");
            viewProductDetails(productInventory[productIndex]);
            return;
        }
    }

    // if product not found
    printf("Product Not Found!\n");
}

// helper method to search the product name by partial match
int isProductNameFound(char *productName, char *searchName)
{
    int Index = 0;
    // match the both strings character by character
    while (productName[Index] != '\0' && searchName[Index] != '\0')
    {
        if (productName[Index] != searchName[Index])
        {
            return 0;
        }
        Index++;
    }
    if (searchName[Index] == '\0')
        return 1;

    return 0;
}

// helper method to search the product by Name
void searchProductByName()
{
    // get the name of the product from user
    char searchProductName[50];
    printf("Enter name to search (partial allowed): ");
    scanf("%s", searchProductName);
    // search the product name in inventory
    int productNotFound = 1;
    for (int productIndex = 0; productIndex < inventorySize; productIndex++)
    {
        if (isProductNameFound(productInventory[productIndex].productName, searchProductName))
        {
            productNotFound = 0;
            printf("Product Found:\n");
            viewProductDetails(productInventory[productIndex]);
        }
    }
    // product is not found
    if (productNotFound)
    {
        printf("Product Not Found:\n");
    }
}

// helper method to search the product by price range
void searchProductByPriceRange()
{
    // get the price range from user
    float minimumPrice, maximumPrice;
    printf("Enter minimum price: ");
    scanf("%f", &minimumPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maximumPrice);
    // search the product in the price range
    int productNotFound = 1;
    printf("Products in price range:\n");
    for (int productIndex = 0; productIndex < inventorySize; productIndex++)
    {
        if (productInventory[productIndex].price >= minimumPrice && productInventory[productIndex].price <= maximumPrice)
        {
            productNotFound = 0;
            viewProductDetails(productInventory[productIndex]);
        }
    }
    // product is not found
    if (productNotFound)
    {
        printf("No Products Found in the given price range:\n");
    }
}

// helper method to delete a product by ID
int deleteProduct()
{
    // get the product id to delete
    int deleteProductID;
    printf("Enter Product ID to delete: ");
    scanf("%d", &deleteProductID);
    // search for the product and delete it
    for (int productIndex = 0; productIndex < inventorySize; productIndex++)
    {
        if (productInventory[productIndex].productID == deleteProductID)
        {
            // shift the products to fill the gap
            for (int nextProductIndex = productIndex; nextProductIndex < inventorySize - 1; nextProductIndex++)
            {
                productInventory[nextProductIndex] = productInventory[nextProductIndex + 1];
            }
            // decrease inventory size
            inventorySize--;
            // reallocate memory for inventory
            // transfer the productInventory pointer to a temporary pointer to avoid memory leak in case of realloc failure
            struct Product *temporaryInventory = (struct Product *)realloc(productInventory, inventorySize * sizeof(struct Product));
            if (temporaryInventory == NULL && inventorySize > 0)
            {
                printf("Memory reallocation failed!\n");
                return 1;
            }
            // update the productInventory pointer
            productInventory = temporaryInventory;
            printf("Product deleted successfully!\n");
            return 0;
        }
    }
    // if product not found
    printf("Product Not Found!\n");
    return 0;
}

int main()
{
    productInventory = NULL;
    inventorySize = 0;
    // Take initial inventory size input
    printf("Enter the initial number of products: ");
    scanf("%d", &inventorySize);
    // allocate memory for inventory
    productInventory = (struct Product *)malloc(inventorySize * sizeof(struct Product));
    // check memory allocation is successful or not
    if (productInventory == NULL)
    {
        printf("Memory allocation failed!\n");
        return 0;
    }
    // Take product details input
    for (int productIndex = 0; productIndex < inventorySize; productIndex++)
    {
        printf("\n");
        printf("Enter details for product %d:\n", productIndex + 1);
        printf("Product ID: ");
        scanf("%d", &productInventory[productIndex].productID);
        printf("Product Name: ");
        scanf("%s", productInventory[productIndex].productName);
        printf("Product Price: ");
        scanf("%f", &productInventory[productIndex].price);
        printf("Product Quantity: ");
        scanf("%d", &productInventory[productIndex].quantity);
    }

    // print the inventry menu until user chooses to exit
    int choice;
    do
    {
        printf("\n");
        printf("========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n");
        switch (choice)
        {
        case 1:
            // Add New Product
            if (addNewProduct())
                choice = 8;
            break;
        case 2:
            // View All Products
            viewAllProducts();
            break;
        case 3:
            // Update Quantity
            updateQuantity();
            break;
        case 4:
            // Search Product by ID
            searchProductByID();
            break;
        case 5:
            // Search Product by Name
            searchProductByName();
            break;
        case 6:
            // Search Product by Price Range
            searchProductByPriceRange();
            break;
        case 7:
            // Delete Product
            if (deleteProduct())
                choice = 8;
            break;
        case 8:
            // Exit
            free(productInventory);
            productInventory = NULL;
            printf("Memory released successfully. Exiting program...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 8);

    return 0;
}