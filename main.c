#include <stdio.h>
#include <string.h>

#define MAX_ZONES 10
#define MAX_QRS 50

// 1. Dữ liệu cấu trúc
struct TicketZone {
    int zone_id;
    char zone_name[20];
    float ticket_price;
    int total_capacity;
    int sold_tickets;
};

struct QrCheckIn {
    char qr_code[20];
    int zone_id;
    int is_used;
};

// 2. Nguyên mẫu hàm (Function Prototypes)
int findZoneIndex(const struct TicketZone zones[], int total_zones, int zone_id);
int findQrIndex(const struct QrCheckIn qrs[], int total_qrs, const char* qr_code);

// Sử dụng con trỏ để trực tiếp thay đổi trạng thái của 1 Zone / QR cụ thể
float processTicketSale(struct TicketZone *zone, int quantity, int is_early_bird);
int processCheckIn(struct QrCheckIn *qr);

int main() {
    // Khởi tạo dữ liệu hệ thống
    struct TicketZone zones[MAX_ZONES] = {
        {1, "VIP", 2000000.0f, 100, 98}, // Gần đầy
        {2, "Zone A", 1200000.0f, 200, 50}
    };
    int total_zones = 2;

    struct QrCheckIn qrs[MAX_QRS] = {
        {"QR-VIP-001", 1, 0},
        {"QR-A-001", 2, 1} // Đã check-in
    };
    int total_qrs = 2;

    printf("=== HE THONG BAN VE TICKETBOX LIVE ===\n\n");

    // Kịch bản 1: Lỗi số lượng không hợp lệ (quantity = 5)
    printf("[Test 1] Mua 5 ve VIP:\n");
    if (findZoneIndex(zones, total_zones, 1) != -1) {
        float price = processTicketSale(&zones[findZoneIndex(zones, total_zones, 1)], 5, 0);
    }

    // Kịch bản 2: Lỗi tràn dung lượng (Mua 3 vé VIP trong khi chỉ còn 2)
    printf("\n[Test 2] Mua 3 ve VIP (Con ton: 2):\n");
    float price2 = processTicketSale(&zones[0], 3, 0);

    // Kịch bản 3: Mã khu vực không tồn tại
    printf("\n[Test 3] Mua ve khu vuc Ma 99 (Khong ton tai):\n");
    int idx99 = findZoneIndex(zones, total_zones, 99);
    if (idx99 == -1) {
        printf("[LOI]: Ma khu vuc 99 khong ton tai tren he thong!\n");
    }

    // Kịch bản 4: Mua hợp lệ + Early Bird (Mua 2 vé Zone A)
    printf("\n[Test 4] Mua 2 ve Zone A (Early Bird):\n");
    float price4 = processTicketSale(&zones[1], 2, 1);
    if (price4 > 0) {
        printf("[THANH CONG]: Giao dich hoan tat. Tong tien: %.0f VND\n", price4);
        printf("[INFO]: So ve da ban cua Zone A hien tai: %d\n", zones[1].sold_tickets);
    }

    // Kịch bản 5: Lỗi Double Check-in
    printf("\n[Test 5] Quet QR-A-001 (Da su dung truoc do):\n");
    int qrIdx = findQrIndex(qrs, total_qrs, "QR-A-001");
    if (qrIdx != -1) {
        processCheckIn(&qrs[qrIdx]);
    }

    // Kịch bản 6: Check-in hợp lệ
    printf("\n[Test 6] Quet QR-VIP-001 (Chua su dung):\n");
    qrIdx = findQrIndex(qrs, total_qrs, "QR-VIP-001");
    if (qrIdx != -1) {
        processCheckIn(&qrs[qrIdx]);
    }

    return 0;
}

// --- TRIỂN KHAI HÀM ---

int findZoneIndex(const struct TicketZone zones[], int total_zones, int zone_id) {
    if (total_zones == 0) return -1;
    for (int i = 0; i < total_zones; i++) {
        if (zones[i].zone_id == zone_id) return i;
    }
    return -1;
}

int findQrIndex(const struct QrCheckIn qrs[], int total_qrs, const char* qr_code) {
    if (total_qrs == 0) return -1;
    for (int i = 0; i < total_qrs; i++) {
        if (strcmp(qrs[i].qr_code, qr_code) == 0) return i;
    }
    return -1;
}

// Hàm nhận con trỏ để thay đổi trực tiếp sold_tickets
float processTicketSale(struct TicketZone *zone, int quantity, int is_early_bird) {
    // 1. Kiem tra gioi han mua
    if (quantity < 1 || quantity > 4) {
        printf("[LOI]: So luong ve (%d) khong hop le. Chi duoc mua 1-4 ve/giao dich.\n", quantity);
        return -1.0f;
    }

    // 2. Kiem tra suc chua (Overbooking)
    if (zone->sold_tickets + quantity > zone->total_capacity) {
        printf("[LOI]: Khu vuc '%s' khong du cho. Con trong: %d, Yeu cau: %d.\n", 
               zone->zone_name, zone->total_capacity - zone->sold_tickets, quantity);
        return -1.0f;
    }

    // 3. Tinh toan tien
    float final_price = zone->ticket_price * quantity;
    if (is_early_bird == 1) {
        final_price *= 0.85f; // Giam 15%
    }

    // 4. Cap nhat trang thai qua con tro
    zone->sold_tickets += quantity;

    return final_price;
}

// Hàm nhận con trỏ để thay đổi trực tiếp is_used
int processCheckIn(struct QrCheckIn *qr) {
    if (qr->is_used == 1) {
        printf("[CANH BAO]: Ma QR '%s' da duoc quet vao cong roi. TU CHOI TRUY CAP!\n", qr->qr_code);
        return 0; // That bai
    }

    qr->is_used = 1; // Danh dau da check-in
    printf("[THANH CONG]: Quet QR '%s' hop le. Xin chao mung!\n", qr->qr_code);
    return 1; // Thanh cong
}