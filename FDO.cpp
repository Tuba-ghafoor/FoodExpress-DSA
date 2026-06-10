#include <iostream>
#include <string>
#include <stack>
#include <queue>
const int MAX_LOCATIONS = 20;

using namespace std;

//==================== ORDER STRUCTURE ====================

struct Order
{
    int orderID;
    string customerName;
    string restaurantName;

    int priority;
    int prepTime;
    int deadline;

    string status;
};

struct Kitchen
{
    int kitchenID;
    string kitchenName;
    int capacity;
    int currentLoad;
    int assignedOrders[100];
    int orderCount;
};

struct Rider
{
    int riderID;
    string riderName;
    int load;
    int distance;
    bool available;
    int completedDeliveries;
    int assignedOrders[100];
    int orderCount;
};

//==================== CREATE ORDER ====================

Order createOrder()
{
    Order o;

    cout << "\nEnter Order ID: ";
    cin >> o.orderID;

    cin.ignore();

    cout << "Customer Name: ";
    getline(cin, o.customerName);

    cout << "Restaurant Name: ";
    getline(cin, o.restaurantName);

    cout << "Priority (1-100): ";
    cin >> o.priority;

    cout << "Preparation Time (minutes): ";
    cin >> o.prepTime;

    cout << "Deadline (minutes): ";
    cin >> o.deadline;

    o.status = "Placed";

    return o;
}

//==================== MAX HEAP ====================

class MaxHeap
{
private:
    Order heap[1000];
    int size;

public:

    MaxHeap()
    {
        size = 0;
    }

    bool isEmpty()
    {
        return size == 0;
    }

    void swapOrders(Order& a, Order& b)
    {
        Order temp = a;
        a = b;
        b = temp;
    }

    void heapifyUp(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;

            if (heap[index].priority > heap[parent].priority)
            {
                swapOrders(heap[index], heap[parent]);
                index = parent;
            }
            else
            {
                break;
            }
        }
    }

    void heapifyDown(int index)
    {
        while (true)
        {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size &&
                heap[left].priority > heap[largest].priority)
            {
                largest = left;
            }

            if (right < size &&
                heap[right].priority > heap[largest].priority)
            {
                largest = right;
            }

            if (largest != index)
            {
                swapOrders(heap[index], heap[largest]);
                index = largest;
            }
            else
            {
                break;
            }
        }
    }

    void insert(Order o)
    {
        if (size >= 1000)
        {
            cout << "\nHeap Full!\n";
            return;
        }

        heap[size] = o;
        heapifyUp(size);
        size++;

        cout << "\nOrder Inserted Successfully!\n";
    }

    Order getMax()
    {
        return heap[0];
    }

    void deleteMax()
    {
        if (isEmpty())
        {
            cout << "\nNo Orders Available!\n";
            return;
        }

        cout << "\nProcessing Order ID: "
            << heap[0].orderID << endl;

        heap[0] = heap[size - 1];
        size--;

        heapifyDown(0);
    }
    void cancelOrder(int id)
    {
        int index = -1;

        for (int i = 0; i < size; i++)
        {
            if (heap[i].orderID == id)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            cout << "\nOrder Not Found!\n";
            return;
        }

        heap[index] = heap[size - 1];
        size--;

        if (index < size)
        {
            heapifyUp(index);
            heapifyDown(index);
        }

        cout << "\nOrder Cancelled Successfully!\n";
    }

    void updatePriority(int id, int newPriority)
    {
        int index = -1;

        for (int i = 0; i < size; i++)
        {
            if (heap[i].orderID == id)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            cout << "\nOrder Not Found!\n";
            return;
        }

        int oldPriority = heap[index].priority;

        heap[index].priority = newPriority;

        if (newPriority > oldPriority)
            heapifyUp(index);
        else
            heapifyDown(index);

        cout << "\nPriority Updated Successfully!\n";
    }

    void display()
    {
        if (size == 0)
        {
            cout << "\nNo Orders Available!\n";
            return;
        }

        cout << "\n========== ORDERS ==========\n";

        for (int i = 0; i < size; i++)
        {
            cout << "\nOrder ID: "
                << heap[i].orderID;

            cout << "\nCustomer: "
                << heap[i].customerName;

            cout << "\nRestaurant: "
                << heap[i].restaurantName;

            cout << "\nPriority: "
                << heap[i].priority;

            cout << "\nPrep Time: "
                << heap[i].prepTime;

            cout << "\nDeadline: "
                << heap[i].deadline;

            cout << "\nStatus: "
                << heap[i].status;

            cout << "\n--------------------------";
        }
    }
    void searchOrder(int id)
    {
        for (int i = 0; i < size; i++)
        {
            if (heap[i].orderID == id)
            {
                cout << "\n===== ORDER FOUND =====\n";

                cout << "Order ID: "
                    << heap[i].orderID << endl;

                cout << "Customer: "
                    << heap[i].customerName << endl;

                cout << "Restaurant: "
                    << heap[i].restaurantName << endl;

                cout << "Priority: "
                    << heap[i].priority << endl;

                cout << "Status: "
                    << heap[i].status << endl;

                return;
            }
        }

        cout << "\nOrder Not Found!\n";
    }
};

class KitchenManager
{
private:

    Kitchen kitchens[50];
    int totalKitchens;

public:

    KitchenManager()
    {
        totalKitchens = 0;
    }

    void addKitchen()
    {
        Kitchen k;

        cout << "\nEnter Kitchen ID: ";
        cin >> k.kitchenID;

        cin.ignore();

        cout << "Kitchen Name: ";
        getline(cin, k.kitchenName);

        cout << "Capacity: ";
        cin >> k.capacity;

        k.currentLoad = 0;
        k.orderCount = 0;

        kitchens[totalKitchens++] = k;

        cout << "\nKitchen Added Successfully!\n";
    }

    void displayLoads()
    {
        if (totalKitchens == 0)
        {
            cout << "\nNo Kitchens Available!\n";
            return;
        }

        cout << "\n====== KITCHEN LOADS ======\n";

        for (int i = 0; i < totalKitchens; i++)
        {
            cout << "\nID: " << kitchens[i].kitchenID;
            cout << "\nName: " << kitchens[i].kitchenName;
            cout << "\nCapacity: " << kitchens[i].capacity;
            cout << "\nCurrent Load: " << kitchens[i].currentLoad;
            cout << "\nAssigned Orders: ";

            for (int j = 0; j < kitchens[i].orderCount; j++)
            {
                cout << kitchens[i].assignedOrders[j] << " ";
            }
            cout << "\n-----------------------";
        }
    }
    void searchKitchen(int id)
    {
        for (int i = 0; i < totalKitchens; i++)
        {
            if (kitchens[i].kitchenID == id)
            {
                cout << "\n===== KITCHEN FOUND =====\n";

                cout << "ID: "
                    << kitchens[i].kitchenID << endl;

                cout << "Name: "
                    << kitchens[i].kitchenName << endl;

                cout << "Capacity: "
                    << kitchens[i].capacity << endl;

                cout << "Current Load: "
                    << kitchens[i].currentLoad
                    << endl;

                return;
            }
        }

        cout << "\nKitchen Not Found!\n";
    }

    void assignOrder()
    {
        int kitchenID;
        int orderID;

        cout << "\nEnter Kitchen ID: ";
        cin >> kitchenID;

        cout << "Enter Order ID: ";
        cin >> orderID;

        for (int i = 0; i < totalKitchens; i++)
        {
            if (kitchens[i].kitchenID == kitchenID)
            {
                kitchens[i].assignedOrders[
                    kitchens[i].orderCount
                ] = orderID;

                kitchens[i].orderCount++;

                kitchens[i].currentLoad++;

                cout << "\nOrder "
                    << orderID
                    << " Assigned To Kitchen "
                    << kitchenID << endl;

                return;
            }
        }

        cout << "\nKitchen Not Found!\n";
    }
    void detectOverloaded()
    {
        bool found = false;

        cout << "\n====== OVERLOADED KITCHENS ======\n";

        for (int i = 0; i < totalKitchens; i++)
        {
            double loadPercent =
                ((double)kitchens[i].currentLoad /
                    kitchens[i].capacity) * 100;

            if (loadPercent >= 80)
            {
                found = true;

                cout << "\nKitchen: "
                    << kitchens[i].kitchenName;

                cout << "\nLoad: "
                    << loadPercent
                    << "%\n";
            }
        }

        if (!found)
        {
            cout << "\nNo Overloaded Kitchens.\n";
        }
    }
    void estimateWaitingTime()
    {
        int id;

        cout << "\nEnter Kitchen ID: ";
        cin >> id;

        for (int i = 0; i < totalKitchens; i++)
        {
            if (kitchens[i].kitchenID == id)
            {
                int waitTime =
                    kitchens[i].currentLoad * 10;

                cout << "\nEstimated Waiting Time: "
                    << waitTime
                    << " minutes\n";

                return;
            }
        }

        cout << "\nKitchen Not Found!\n";
    }
    int getTotalKitchens()
    {
        return totalKitchens;
    }
};

class RiderManager
{
private:

    Rider riders[50];
    int totalRiders;

public:

    RiderManager()
    {
        totalRiders = 0;
    }

    void addRider()
    {
        Rider r;

        cout << "\nEnter Rider ID: ";
        cin >> r.riderID;

        cin.ignore();

        cout << "Rider Name: ";
        getline(cin, r.riderName);

        cout << "Current Load: ";
        cin >> r.load;

        cout << "Distance From Restaurant (km): ";
        cin >> r.distance;

        r.available = true;

        r.completedDeliveries = 0;
        r.orderCount = 0;

        riders[totalRiders++] = r;

        cout << "\nRider Added Successfully!\n";
    }

    void displayRiders()
    {
        if (totalRiders == 0)
        {
            cout << "\nNo Riders Available!\n";
            return;
        }

        for (int i = 0; i < totalRiders; i++)
        {
            cout << "\nID: " << riders[i].riderID;
            cout << "\nName: " << riders[i].riderName;
            cout << "\nLoad: " << riders[i].load;
            cout << "\nDistance: " << riders[i].distance;
            cout << "\nAvailable: "
                << (riders[i].available ? "Yes" : "No");
            cout << "\nAssigned Orders: ";

            for (int j = 0; j < riders[i].orderCount; j++)
            {
                cout << riders[i].assignedOrders[j]
                    << " ";
            }
            cout << "\n----------------------";

        }
    }

    void searchRider(int id)
    {
        for (int i = 0; i < totalRiders; i++)
        {
            if (riders[i].riderID == id)
            {
                cout << "\n===== RIDER FOUND =====\n";

                cout << "ID: "
                    << riders[i].riderID << endl;

                cout << "Name: "
                    << riders[i].riderName << endl;

                cout << "Load: "
                    << riders[i].load << endl;

                cout << "Distance: "
                    << riders[i].distance << endl;

                cout << "Completed Deliveries: "
                    << riders[i].completedDeliveries
                    << endl;

                return;
            }
        }

        cout << "\nRider Not Found!\n";
    }

    void assignBestRider()
    {
        int orderID;

        cout << "\nEnter Order ID: ";
        cin >> orderID;
        if (totalRiders == 0)
        {
            cout << "\nNo Riders Available!\n";
            return;
        }

        int bestIndex = -1;

        for (int i = 0; i < totalRiders; i++)
        {
            if (riders[i].available)
            {
                if (bestIndex == -1)
                {
                    bestIndex = i;
                }
                else
                {
                    if (riders[i].load < riders[bestIndex].load)
                    {
                        bestIndex = i;
                    }
                    else if (riders[i].load == riders[bestIndex].load)
                    {
                        if (riders[i].distance <
                            riders[bestIndex].distance)
                        {
                            bestIndex = i;
                        }
                    }
                }
            }
        }

        if (bestIndex == -1)
        {
            cout << "\nNo Available Rider!\n";
            return;
        }

        riders[bestIndex].load++;
        riders[bestIndex].assignedOrders[
            riders[bestIndex].orderCount
        ] = orderID;

        riders[bestIndex].orderCount++;

        cout << "\n===== BEST RIDER ASSIGNED =====\n";

        cout << "Rider ID: "
            << riders[bestIndex].riderID << endl;

        cout << "Name: "
            << riders[bestIndex].riderName << endl;

        cout << "Current Load: "
            << riders[bestIndex].load << endl;

        cout << "Distance: "
            << riders[bestIndex].distance << " km\n";
        cout << "Assigned Order: "
            << orderID << endl;
    }
    void completeDelivery(int riderID)
    {
        for (int i = 0; i < totalRiders; i++)
        {
            if (riders[i].riderID == riderID)
            {
                if (riders[i].load > 0)
                    riders[i].load--;

                riders[i].completedDeliveries++;

                cout << "\nDelivery Completed!\n";
                return;
            }
        }

        cout << "\nRider Not Found!\n";
    }
    int getTotalRiders()
    {
        return totalRiders;
    }
};

class Graph
{
private:

    int adjMatrix[MAX_LOCATIONS][MAX_LOCATIONS];
    int vertices;
    string locations[MAX_LOCATIONS];

public:

    Graph()
    {
        vertices = 0;

        for (int i = 0; i < MAX_LOCATIONS; i++)
        {
            for (int j = 0; j < MAX_LOCATIONS; j++)
            {
                adjMatrix[i][j] = 0;
            }
        }
    }

    void setVertices(int v)
    {
        vertices = v;
    }
    int getLocationIndex(string city)
    {
        for (int i = 0; i < vertices; i++)
        {
            if (locations[i] == city)
                return i;
        }

        return -1;
    }
    string getLocationName(int index)
    {
        return locations[index];
    }

    void setLocation(int index, string name)
    {
        locations[index] = name;
    }

    void addRoad(int source,
        int destination,
        int distance)
    {
        adjMatrix[source][destination] = distance;
        adjMatrix[destination][source] = distance;

        cout << "\nRoad Added Successfully!\n";
    }

    void displayMap()
    {
        cout << "\n===== CITY MAP =====\n";

        for (int i = 0; i < vertices; i++)
        {
            for (int j = i + 1; j < vertices; j++)
            {
                if (adjMatrix[i][j] != 0)
                {
                    cout << locations[i]
                        << " -> "
                        << locations[j]
                        << " ("
                        << adjMatrix[i][j]
                        << " km)"
                        << endl;
                }
            }
        }
    }
    void shortestRoute(int source)
    {
        int distance[MAX_LOCATIONS];
        bool visited[MAX_LOCATIONS];

        for (int i = 0; i < vertices; i++)
        {
            distance[i] = 999999;
            visited[i] = false;
        }

        distance[source] = 0;

        for (int count = 0; count < vertices - 1; count++)
        {
            int minDistance = 999999;
            int u = -1;

            for (int i = 0; i < vertices; i++)
            {
                if (!visited[i] &&
                    distance[i] < minDistance)
                {
                    minDistance = distance[i];
                    u = i;
                }
            }

            if (u == -1)
                break;

            visited[u] = true;

            for (int v = 0; v < vertices; v++)
            {
                if (adjMatrix[u][v] != 0 &&
                    !visited[v] &&
                    distance[u] + adjMatrix[u][v]
                    < distance[v])
                {
                    distance[v] =
                        distance[u] + adjMatrix[u][v];
                }
            }
        }

        cout << "\n===== SHORTEST ROUTES =====\n";

        for (int i = 0; i < vertices; i++)
        {
            cout << locations[source]
                << " -> "
                << locations[i]
                << " = "
                << distance[i]
                << " km"
                << endl;
        }
    }
};

class HistoryManager
{
private:
    int pendingOrders;
    int completedOrders;
    int cancelledOrders;

public:
    HistoryManager()
    {
        pendingOrders = 0;
        completedOrders = 0;
        cancelledOrders = 0;
    }

    void addPending()
    {
        pendingOrders++;
    }

    void addCompleted()
    {
        completedOrders++;
        if (pendingOrders > 0)
            pendingOrders--;
    }

    void addCancelled()
    {
        cancelledOrders++;
        if (pendingOrders > 0)
            pendingOrders--;
    }

    void showReport()
    {
        cout << "\n===== ORDER HISTORY REPORT =====\n";

        cout << "\nPending Orders: "
            << pendingOrders;

        cout << "\nCompleted Orders: "
            << completedOrders;

        cout << "\nCancelled Orders: "
            << cancelledOrders << endl;
    }
    int getPendingOrders()
    {
        return pendingOrders;
    }

    int getCompletedOrders()
    {
        return completedOrders;
    }

    int getCancelledOrders()
    {
        return cancelledOrders;
    }

    int getTotalOrders()
    {
        return pendingOrders +
            completedOrders +
            cancelledOrders;
    }
};
//==================== STACK ====================

class OrderStack
{
private:
    int stack[1000];
    int top;

public:
    OrderStack()
    {
        top = -1;
    }

    void push(int orderID)
    {
        if (top < 999)
        {
            stack[++top] = orderID;
        }
    }

    void pop()
    {
        if (top >= 0)
        {
            top--;
        }
    }

    bool isEmpty()
    {
        return top == -1;
    }
};

//==================== QUEUE ====================

class OrderQueue
{
private:
    int queue[1000];
    int front;
    int rear;

public:
    OrderQueue()
    {
        front = 0;
        rear = -1;
    }

    void enqueue(int orderID)
    {
        if (rear < 999)
        {
            queue[++rear] = orderID;
        }
    }

    void dequeue()
    {
        if (front <= rear)
        {
            front++;
        }
    }

    bool isEmpty()
    {
        return front > rear;
    }
};

//==================== GLOBAL HEAP ====================

MaxHeap orderHeap;
KitchenManager kitchenManager;
RiderManager riderManager;
Graph cityGraph;
HistoryManager historyManager;
OrderStack orderStack;
OrderQueue orderQueue;


//==================== DYNAMIC ORDER MENU ====================

void dynamicOrderScheduling()
{
    int choice;

    do
    {
        cout << "\n================================";
        cout << "\n Dynamic Order Scheduling";
        cout << "\n================================";
        cout << "\n1. Add Order";
        cout << "\n2. Cancel Order";
        cout << "\n3. Update Priority";
        cout << "\n4. Show Highest Priority Order";
        cout << "\n5. Process Next Order";
        cout << "\n6. Display Orders";
        cout << "\n7. Back";
        cout << "\nEnter Choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Order o = createOrder();
            orderHeap.insert(o);

            orderStack.push(o.orderID);
            orderQueue.enqueue(o.orderID);

            historyManager.addPending();

            break;
        }

        case 2:
        {
            int id;

            cout << "\nEnter Order ID To Cancel: ";
            cin >> id;

            orderHeap.cancelOrder(id);
            if (!orderStack.isEmpty())
                orderStack.pop();

            historyManager.addCancelled();

            break;
        }

        case 3:
        {
            int id;
            int newPriority;

            cout << "\nEnter Order ID: ";
            cin >> id;

            cout << "Enter New Priority: ";
            cin >> newPriority;

            orderHeap.updatePriority(id, newPriority);

            break;
        }

        case 4:
        {
            if (orderHeap.isEmpty())
            {
                cout << "\nNo Orders Available!\n";
            }
            else
            {
                Order top = orderHeap.getMax();

                cout << "\nHighest Priority Order\n";

                cout << "Order ID: "
                    << top.orderID << endl;

                cout << "Customer: "
                    << top.customerName << endl;

                cout << "Priority: "
                    << top.priority << endl;
            }
            break;
        }

        case 5:
            orderHeap.deleteMax();

            if (!orderQueue.isEmpty())
                orderQueue.dequeue();

            historyManager.addCompleted();
            break;

        case 6:
            orderHeap.display();
            break;

        case 7:
            cout << "\nReturning To Main Menu...\n";
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 7);
}

void kitchenLoadAnalysis()
{
    int choice;

    do
    {
        cout << "\n================================";
        cout << "\n Kitchen Load Analysis";
        cout << "\n================================";
        cout << "\n1. Add Kitchen";
        cout << "\n2. Assign Order To Kitchen";
        cout << "\n3. View Kitchen Loads";
        cout << "\n4. Detect Overloaded Kitchens";
        cout << "\n5. Estimate Waiting Time";
        cout << "\n6. Back";
        cout << "\nEnter Choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            kitchenManager.addKitchen();
            break;

        case 2:
            kitchenManager.assignOrder();
            break;

        case 3:
            kitchenManager.displayLoads();
            break;

        case 4:
            kitchenManager.detectOverloaded();
            break;

        case 5:
            kitchenManager.estimateWaitingTime();
            break;

        case 6:
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 6);
}
void riderDispatchOptimization()
{
    int choice;

    do
    {
        cout << "\n================================";
        cout << "\n Rider Dispatch Optimization";
        cout << "\n================================";
        cout << "\n1. Add Rider";
        cout << "\n2. Assign Best Rider";
        cout << "\n3. View Riders";
        cout << "\n4. Complete Delivery";
        cout << "\n5. Back";
        cout << "\nEnter Choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            riderManager.addRider();
            break;

        case 2:
            riderManager.assignBestRider();
            break;

        case 3:
            riderManager.displayRiders();
            break;

        case 4:
        {
            int id;

            cout << "\nEnter Rider ID: ";
            cin >> id;

            riderManager.completeDelivery(id);

            break;
        }
        case 5:
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 5);
}
void routeOptimization()
{
    int choice;

    do
    {
        cout << "\n================================";
        cout << "\n Route Optimization";
        cout << "\n================================";
        cout << "\n1. Set Locations";
        cout << "\n2. Add Road";
        cout << "\n3. Display City Map";
        cout << "\n4. Find Shortest Route";
        cout << "\n5. Back";
        cout << "\nEnter Choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int v;

            cout << "\nEnter Number Of Locations: ";
            cin >> v;

            cityGraph.setVertices(v);

            cin.ignore();

            for (int i = 0; i < v; i++)
            {
                string city;

                cout << "Enter Location "
                    << i + 1
                    << ": ";

                getline(cin, city);

                cityGraph.setLocation(i, city);
            }

            break;
        }

        case 2:
        {
            string source;
            string destination;
            int dist;

            cin.ignore();

            cout << "\nSource City: ";
            getline(cin, source);

            cout << "Destination City: ";
            getline(cin, destination);

            cout << "Distance: ";
            cin >> dist;

            int src =
                cityGraph.getLocationIndex(source);

            int dest =
                cityGraph.getLocationIndex(destination);

            if (src == -1 || dest == -1)
            {
                cout << "\nCity Not Found!\n";
            }
            else
            {
                cityGraph.addRoad(src, dest, dist);
            }

            break;
        }

        case 3:
            cityGraph.displayMap();
            break;

        case 4:
        {
            string sourceCity;

            cin.ignore();

            cout << "\nEnter Source City: ";
            getline(cin, sourceCity);

            int source =
                cityGraph.getLocationIndex(sourceCity);

            if (source == -1)
            {
                cout << "\nCity Not Found!\n";
            }
            else
            {
                cityGraph.shortestRoute(source);
            }

            break;
        }

        case 5:
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 5);
}
void searchAndRetrievalEngine()
{
    int choice;

    do
    {
        cout << "\n================================";
        cout << "\n Search And Retrieval Engine";
        cout << "\n================================";
        cout << "\n1. Search Order";
        cout << "\n2. Search Rider";
        cout << "\n3. Search Kitchen";
        cout << "\n4. Back";
        cout << "\nEnter Choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int id;

            cout << "\nEnter Order ID: ";
            cin >> id;

            orderHeap.searchOrder(id);

            break;
        }

        case 2:
        {
            int id;

            cout << "\nEnter Rider ID: ";
            cin >> id;

            riderManager.searchRider(id);

            break;
        }

        case 3:
        {
            int id;

            cout << "\nEnter Kitchen ID: ";
            cin >> id;

            kitchenManager.searchKitchen(id);

            break;
        }

        case 4:
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 4);
}
void orderHistoryTracking()
{
    historyManager.showReport();
}
void performanceAnalysis()
{
    cout << "\n===== PERFORMANCE ANALYSIS =====\n";

    cout << "\nOrder Scheduling (Max Heap)";
    cout << "\nInsert Order: O(log n)";
    cout << "\nDelete Order: O(log n)";
    cout << "\nSearch Order: O(n)";

    cout << "\n\nKitchen Load Balancing";
    cout << "\nAssign Kitchen: O(n)";

    cout << "\n\nRider Dispatch";
    cout << "\nFind Best Rider: O(n)";

    cout << "\n\nRoute Optimization";
    cout << "\nShortest Route (Dijkstra): O(V^2)\n";
}
void scalabilitySimulation()
{
    cout << "\n===== SCALABILITY SIMULATION =====\n";

    int totalOrders = historyManager.getTotalOrders();

    cout << "\nCurrent Orders In System: "
        << totalOrders;

    if (totalOrders < 10)
    {
        cout << "\nSystem Load: LOW";
    }
    else if (totalOrders < 50)
    {
        cout << "\nSystem Load: MEDIUM";
    }
    else
    {
        cout << "\nSystem Load: HIGH";
    }

    cout << "\nEstimated Performance: Stable\n";
}
void systemReports()
{
    cout << "\n===== SYSTEM REPORT =====\n";

    cout << "\nTotal Orders: "
        << historyManager.getTotalOrders();

    cout << "\nPending Orders: "
        << historyManager.getPendingOrders();

    cout << "\nCompleted Orders: "
        << historyManager.getCompletedOrders();

    cout << "\nCancelled Orders: "
        << historyManager.getCancelledOrders();

    cout << "\nTotal Kitchens: "
        << kitchenManager.getTotalKitchens();

    cout << "\nTotal Riders: "
        << riderManager.getTotalRiders();
    cout << endl;
}

//==================== MAIN ====================

int main()
{
    int choice;

    do
    {
        cout << "\n=========================================";
        cout << "\n FoodExpress Dispatch Optimization Engine";
        cout << "\n=========================================";
        cout << "\n1. Dynamic Order Scheduling";
        cout << "\n2. Kitchen Load Analysis";
        cout << "\n3. Rider Dispatch Optimization";
        cout << "\n4. Route Optimization";
        cout << "\n5. Search and Retrieval Engine";
        cout << "\n6. Order History Tracking";
        cout << "\n7. Performance Analysis";
        cout << "\n8. Scalability Simulation";
        cout << "\n9. System Reports";
        cout << "\n10. Exit";
        cout << "\n=========================================";
        cout << "\nEnter Choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            dynamicOrderScheduling();
            break;

        case 2:
            kitchenLoadAnalysis();
            break;

        case 3:
            riderDispatchOptimization();
            break;

        case 4:
            routeOptimization();
            break;

        case 5:
            searchAndRetrievalEngine();
            break;

        case 6:
            orderHistoryTracking();
            break;

        case 7:
            performanceAnalysis();

            break;

        case 8:
            scalabilitySimulation();
            break;

        case 9:
            systemReports();
            break;

        case 10:
            cout << "\nExiting System...\n";
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 10);

    return 0;
}