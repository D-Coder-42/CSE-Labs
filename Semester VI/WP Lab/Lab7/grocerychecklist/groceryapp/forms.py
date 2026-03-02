from django import forms

# Dictionary holding the items and their prices
GROCERY_ITEMS = {
    'Milk': 2.50,
    'Bread': 1.50,
    'Eggs': 3.00,
    'Apples': 2.00,
    'Bananas': 1.20,
    'Coffee': 5.00,
    'Chicken': 7.50
}

# Format the dictionary keys into a tuple of choices for the form
CHOICES = [(item, f"{item} (${price:.2f})") for item, price in GROCERY_ITEMS.items()]

class GroceryForm(forms.Form):
    items = forms.MultipleChoiceField(
        choices=CHOICES,
        widget=forms.CheckboxSelectMultiple,
        label="Available Groceries"
    )