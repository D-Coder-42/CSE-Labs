from django.shortcuts import render
from .forms import GroceryForm, GROCERY_ITEMS

def index(request):
    selected_items_data = []
    total_price = 0

    if request.method == 'POST':
        form = GroceryForm(request.POST)
        if form.is_valid():
            selected_items = form.cleaned_data['items']
            # Match selected items with their prices
            for item in selected_items:
                price = GROCERY_ITEMS[item]
                selected_items_data.append({'name': item, 'price': price})
                total_price += price
    else:
        form = GroceryForm()

    context = {
        'form': form,
        'selected_items_data': selected_items_data,
        'total_price': total_price
    }
    return render(request, 'index.html', context)