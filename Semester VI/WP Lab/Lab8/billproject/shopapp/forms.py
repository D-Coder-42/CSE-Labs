from django import forms

class ItemForm(forms.Form):
    BRAND_CHOICES = [
        ('HP', 'HP'),
        ('Nokia', 'Nokia'),
        ('Samsung', 'Samsung'),
        ('Motorola', 'Motorola'),
        ('Apple', 'Apple'),
    ]
    ITEM_CHOICES = [
        ('Mobile', 'Mobile'),
        ('Laptop', 'Laptop'),
    ]

    brand = forms.ChoiceField(label="Select Brand", choices=BRAND_CHOICES, widget=forms.RadioSelect)
    items = forms.MultipleChoiceField(label="Select Items", choices=ITEM_CHOICES, widget=forms.CheckboxSelectMultiple)
    quantity = forms.IntegerField(label="Enter Quantity", min_value=1)
