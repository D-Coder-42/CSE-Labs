# carapp/forms.py
from django import forms

class CarSearchForm(forms.Form):
    MANUFACTURER_CHOICES = [
        ('Toyota', 'Toyota'),
        ('Honda', 'Honda'),
        ('Ford', 'Ford'),
        ('Tesla', 'Tesla'),
        ('Nissan', 'Nissan'),
        ('Tata', 'Tata'),
        ('Mahindra', 'Mahindra')
    ]
    
    manufacturer = forms.ChoiceField(choices=MANUFACTURER_CHOICES, label="Select Manufacturer")
    model_name = forms.CharField(max_length=100, label="Car Model")