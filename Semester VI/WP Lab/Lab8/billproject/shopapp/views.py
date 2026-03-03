from django.shortcuts import render
from .forms import ItemForm

PRICE = {
    'Mobile': 10000,
    'Laptop': 40000,
}

def select_items(request):
    if request.method == "POST":
        form = ItemForm(request.POST)
        if form.is_valid():
            brand = form.cleaned_data['brand']
            items = form.cleaned_data['items']
            quantity = form.cleaned_data['quantity']

            total = sum(PRICE[item] for item in items) * quantity

            return render(request, "bill.html", {
                "brand": brand,
                "items": items,
                "quantity": quantity,
                "total": total
            })
    else:
        form = ItemForm()
    return render(request, "select_items.html", {"form": form})
